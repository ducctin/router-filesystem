/*
 *
 * device driver for philips saa7134 based TV cards
 * driver core
 *
 * (c) 2001-03 Gerd Knorr <kraxel@bytesex.org> [SuSE Labs]
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/kmod.h>
#include <linux/sound.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/dma-mapping.h>
#include <linux/pm.h>

#include "saa7134-reg.h"
#include "saa7134.h"

MODULE_DESCRIPTION("v4l2 driver module for saa7130/34 based TV cards");
MODULE_AUTHOR("Gerd Knorr <kraxel@bytesex.org> [SuSE Labs]");
MODULE_LICENSE("GPL");

/* ------------------------------------------------------------------ */

static unsigned int irq_debug;
module_param(irq_debug, int, 0644);
MODULE_PARM_DESC(irq_debug,"enable debug messages [IRQ handler]");

static unsigned int core_debug;
module_param(core_debug, int, 0644);
MODULE_PARM_DESC(core_debug,"enable debug messages [core]");

static unsigned int gpio_tracking;
module_param(gpio_tracking, int, 0644);
MODULE_PARM_DESC(gpio_tracking,"enable debug messages [gpio]");

static unsigned int alsa = 1;
module_param(alsa, int, 0644);
MODULE_PARM_DESC(alsa,"enable/disable ALSA DMA sound [dmasound]");

static unsigned int latency = UNSET;
module_param(latency, int, 0444);
MODULE_PARM_DESC(latency,"pci latency timer");

int saa7134_no_overlay=-1;
module_param_named(no_overlay, saa7134_no_overlay, int, 0444);
MODULE_PARM_DESC(no_overlay,"allow override overlay default (0 disables, 1 enables)"
		" [some VIA/SIS chipsets are known to have problem with overlay]");

static unsigned int video_nr[] = {[0 ... (SAA7134_MAXBOARDS - 1)] = UNSET };
static unsigned int vbi_nr[]   = {[0 ... (SAA7134_MAXBOARDS - 1)] = UNSET };
static unsigned int radio_nr[] = {[0 ... (SAA7134_MAXBOARDS - 1)] = UNSET };
static unsigned int tuner[]    = {[0 ... (SAA7134_MAXBOARDS - 1)] = UNSET };
static unsigned int card[]     = {[0 ... (SAA7134_MAXBOARDS - 1)] = UNSET };


module_param_array(video_nr, int, NULL, 0444);
module_param_array(vbi_nr,   int, NULL, 0444);
module_param_array(radio_nr, int, NULL, 0444);
module_param_array(tuner,    int, NULL, 0444);
module_param_array(card,     int, NULL, 0444);

MODULE_PARM_DESC(video_nr, "video device number");
MODULE_PARM_DESC(vbi_nr,   "vbi device number");
MODULE_PARM_DESC(radio_nr, "radio device number");
MODULE_PARM_DESC(tuner,    "tuner type");
MODULE_PARM_DESC(card,     "card type");

DEFINE_MUTEX(saa7134_devlist_lock);
EXPORT_SYMBOL(saa7134_devlist_lock);
LIST_HEAD(saa7134_devlist);
EXPORT_SYMBOL(saa7134_devlist);
static LIST_HEAD(mops_list);
static unsigned int saa7134_devcount;

int (*saa7134_dmasound_init)(struct saa7134_dev *dev);
int (*saa7134_dmasound_exit)(struct saa7134_dev *dev);

#define dprintk(fmt, arg...)	if (core_debug) \
	printk(KERN_DEBUG "%s/core: " fmt, dev->name , ## arg)

void saa7134_track_gpio(struct saa7134_dev *dev, char *msg)
{
	unsigned long mode,status;

	if (!gpio_tracking)
		return;
	/* rising SAA7134_GPIO_GPRESCAN reads the status */
	saa_andorb(SAA7134_GPIO_GPMODE3,SAA7134_GPIO_GPRESCAN,0);
	saa_andorb(SAA7134_GPIO_GPMODE3,SAA7134_GPIO_GPRESCAN,SAA7134_GPIO_GPRESCAN);
	mode   = saa_readl(SAA7134_GPIO_GPMODE0   >> 2) & 0xfffffff;
	status = saa_readl(SAA7134_GPIO_GPSTATUS0 >> 2) & 0xfffffff;
	printk(KERN_DEBUG
	       "%s: gpio: mode=0x%07lx in=0x%07lx out=0x%07lx [%s]\n",
	       dev->name, mode, (~mode) & status, mode & status, msg);
}

void saa7134_set_gpio(struct saa7134_dev *dev, int bit_no, int value)
{
	u32 index, bitval;

	index = 1 << bit_no;
	switch (value) {
	case 0: /* static value */
	case 1:	dprintk("setting GPIO%d to static %d\n", bit_no, value);
		/* turn sync mode off if necessary */
		if (index & 0x00c00000)
			saa_andorb(SAA7134_VIDEO_PORT_CTRL6, 0x0f, 0x00);
		if (value)
			bitval = index;
		else
			bitval = 0;
		saa_andorl(SAA7134_GPIO_GPMODE0 >> 2, index, index);
		saa_andorl(SAA7134_GPIO_GPSTATUS0 >> 2, index, bitval);
		break;
	case 3:	/* tristate */
		dprintk("setting GPIO%d to tristate\n", bit_no);
		saa_andorl(SAA7134_GPIO_GPMODE0 >> 2, index, 0);
		break;
	}
}

/* ------------------------------------------------------------------ */

#if 0
static char *dec1_bits[8] = {
	"DCSTD0", "DCSCT1", "WIPA", "GLIMB",
	"GLIMT", "SLTCA", "HLCK"
};
static char *dec2_bits[8] = {
	"RDCAP", "COPRO", "COLSTR", "TYPE3",
	NULL, "FIDT", "HLVLN", "INTL"
};
static char *scale1_bits[8] = {
	"VID_A", "VBI_A", NULL, NULL, "VID_B", "VBI_B"
};
static char *scale2_bits[8] = {
	"TRERR", "CFERR", "LDERR", "WASRST",
	"FIDSCI", "FIDSCO", "D6^D5", "TASK"
};

static void dump_statusreg(struct saa7134_dev *dev, int reg,
			   char *regname, char **bits)
{
	int value,i;

	value = saa_readb(reg);
	printk(KERN_DEBUG "%s: %s:", dev->name, regname);
	for (i = 7; i >= 0; i--) {
		if (NULL == bits[i])
			continue;
		printk(" %s=%d", bits[i], (value & (1 << i)) ? 1 : 0);
	}
	printk("\n");
}

static void dump_statusregs(struct saa7134_dev *dev)
{
	dump_statusreg(dev,SAA7134_STATUS_VIDEO1,"dec1",dec1_bits);
	dump_statusreg(dev,SAA7134_STATUS_VIDEO2,"dec2",dec2_bits);
	dump_statusreg(dev,SAA7134_SCALER_STATUS0,"scale0",scale1_bits);
	dump_statusreg(dev,SAA7134_SCALER_STATUS1,"scale1",scale2_bits);
}
#endif

/* ----------------------------------------------------------- */
/* delayed request_module                                      */

#if defined(CONFIG_MODULES) && defined(MODULE)

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
static void request_module_async(void *ptr){
	struct saa7134_dev* dev=(struct saa7134_dev*)ptr;
#else
static void request_module_async(struct work_struct *work){
	struct saa7134_dev* dev = container_of(work, struct saa7134_dev, request_module_wk);
#endif
	if (card_is_empress(dev))
		request_module("saa7134-empress");
	if (card_is_dvb(dev))
		request_module("saa7134-dvb");
	if (alsa) {
		if (dev->pci->device != PCI_DEVICE_ID_PHILIPS_SAA7130)
			request_module("saa7134-alsa");
	}
}

static void request_submodules(struct saa7134_dev *dev)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	INIT_WORK(&dev->request_module_wk, request_module_async, (void*)dev);
#else
	INIT_WORK(&dev->request_module_wk, request_module_async);
#endif
	schedule_work(&dev->request_module_wk);
}

#else
#define request_submodules(dev)
#endif /* CONFIG_MODULES */

/* ------------------------------------------------------------------ */

/* nr of (saa7134-)pages for the given buffer size */
static int saa7134_buffer_pages(int size)
{
	size  = PAGE_ALIGN(size);
	size += PAGE_SIZE; /* for non-page-aligned buffers */
	size /= 4096;
	return size;
}

/* calc max # of buffers from size (must not exceed the 4MB virtual
 * address space per DMA channel) */
int saa7134_buffer_count(unsigned int size, unsigned int count)
{
	unsigned int maxcount;

	maxcount = 1024 / saa7134_buffer_pages(size);
	if (count > maxcount)
		count = maxcount;
	return count;
}

int saa7134_buffer_startpage(struct saa7134_buf *buf)
{
	return saa7134_buffer_pages(buf->vb.bsize) * buf->vb.i;
}

unsigned long saa7134_buffer_base(struct saa7134_buf *buf)
{
	unsigned long base;
	struct videobuf_dmabuf *dma=videobuf_to_dma(&buf->vb);

	base  = saa7134_buffer_startpage(buf) * 4096;
	base += dma->sglist[0].offset;
	return base;
}

/* ------------------------------------------------------------------ */

int saa7134_pgtable_alloc(struct pci_dev *pci, struct saa7134_pgtable *pt)
{
	__le32       *cpu;
	dma_addr_t   dma_addr = 0;

	cpu = pci_alloc_consistent(pci, SAA7134_PGTABLE_SIZE, &dma_addr);
	if (NULL == cpu)
		return -ENOMEM;
	pt->size = SAA7134_PGTABLE_SIZE;
	pt->cpu  = cpu;
	pt->dma  = dma_addr;
	return 0;
}

int saa7134_pgtable_build(struct pci_dev *pci, struct saa7134_pgtable *pt,
			  struct scatterlist *list, unsigned int length,
			  unsigned int startpage)
{
	__le32        *ptr;
	unsigned int  i,p;

	BUG_ON(NULL == pt || NULL == pt->cpu);

	ptr = pt->cpu + startpage;
	for (i = 0; i < length; i++, list++)
		for (p = 0; p * 4096 < list->length; p++, ptr++)
			*ptr = cpu_to_le32(sg_dma_address(list) - list->offset);
	return 0;
}

void saa7134_pgtable_free(struct pci_dev *pci, struct saa7134_pgtable *pt)
{
	if (NULL == pt->cpu)
		return;
	pci_free_consistent(pci, pt->size, pt->cpu, pt->dma);
	pt->cpu = NULL;
}

/* ------------------------------------------------------------------ */

void saa7134_dma_free(struct videobuf_queue *q,struct saa7134_buf *buf)
{
	struct videobuf_dmabuf *dma=videobuf_to_dma(&buf->vb);
	BUG_ON(in_interrupt());

	videobuf_waiton(&buf->vb,0,0);
	videobuf_dma_unmap(q, dma);
	videobuf_dma_free(dma);
	buf->vb.state = VIDEOBUF_NEEDS_INIT;
}

/* ------------------------------------------------------------------ */

int saa7134_buffer_queue(struct saa7134_dev *dev,
			 struct saa7134_dmaqueue *q,
			 struct saa7134_buf *buf)
{
	struct saa7134_buf *next = NULL;

	assert_spin_locked(&dev->slock);
	dprintk("buffer_queue %p\n",buf);
	if (NULL == q->curr) {
		if (!q->need_two) {
			q->curr = buf;
			buf->activate(dev,buf,NULL);
		} else if (list_empty(&q->queue)) {
			list_add_tail(&buf->vb.queue,&q->queue);
			buf->vb.state = VIDEOBUF_QUEUED;
		} else {
			next = list_entry(q->queue.next,struct saa7134_buf,
					  vb.queue);
			q->curr = buf;
			buf->activate(dev,buf,next);
		}
	} else {
		list_add_tail(&buf->vb.queue,&q->queue);
		buf->vb.state = VIDEOBUF_QUEUED;
	}
	return 0;
}

void saa7134_buffer_finish(struct saa7134_dev *dev,
			   struct saa7134_dmaqueue *q,
			   unsigned int state)
{
	assert_spin_locked(&dev->slock);
	dprintk("buffer_finish %p\n",q->curr);

	/* finish current buffer */
	q->curr->vb.state = state;
	do_gettimeofday(&q->curr->vb.ts);
	wake_up(&q->curr->vb.done);
	q->curr = NULL;
}

void saa7134_buffer_next(struct saa7134_dev *dev,
			 struct saa7134_dmaqueue *q)
{
	struct saa7134_buf *buf,*next = NULL;

	assert_spin_locked(&dev->slock);
	BUG_ON(NULL != q->curr);

	if (!list_empty(&q->queue)) {
		/* activate next one from queue */
		buf = list_entry(q->queue.next,struct saa7134_buf,vb.queue);
		dprintk("buffer_next %p [prev=%p/next=%p]\n",
			buf,q->queue.prev,q->queue.next);
		list_del(&buf->vb.queue);
		if (!list_empty(&q->queue))
			next = list_entry(q->queue.next,struct saa7134_buf,
					  vb.queue);
		q->curr = buf;
		buf->activate(dev,buf,next);
		dprintk("buffer_next #2 prev=%p/next=%p\n",
			q->queue.prev,q->queue.next);
	} else {
		/* nothing to do -- just stop DMA */
		dprintk("buffer_next %p\n",NULL);
		saa7134_set_dmabits(dev);
		del_timer(&q->timeout);

		if (card_has_mpeg(dev))
			if (dev->ts_started)
				saa7134_ts_stop(dev);
	}
}

void saa7134_buffer_timeout(unsigned long data)
{
	struct saa7134_dmaqueue *q = (struct saa7134_dmaqueue*)data;
	struct saa7134_dev *dev = q->dev;
	unsigned long flags;

	spin_lock_irqsave(&dev->slock,flags);

	/* try to reset the hardware (SWRST) */
	saa_writeb(SAA7134_REGION_ENABLE, 0x00);
	saa_writeb(SAA7134_REGION_ENABLE, 0x80);
	saa_writeb(SAA7134_REGION_ENABLE, 0x00);

	/* flag current buffer as failed,
	   try to start over with the next one. */
	if (q->curr) {
		dprintk("timeout on %p\n",q->curr);
		saa7134_buffer_finish(dev,q,VIDEOBUF_ERROR);
	}
	saa7134_buffer_next(dev,q);
	spin_unlock_irqrestore(&dev->slock,flags);
}

/* ------------------------------------------------------------------ */

int saa7134_set_dmabits(struct saa7134_dev *dev)
{
	u32 split, task=0, ctrl=0, irq=0;
	enum v4l2_field cap = V4L2_FIELD_ANY;
	enum v4l2_field ov  = V4L2_FIELD_ANY;

	assert_spin_locked(&dev->slock);

	if (dev->insuspend)
		return 0;

	/* video capture -- dma 0 + video task A */
	if (dev->video_q.curr) {
		task |= 0x01;
		ctrl |= SAA7134_MAIN_CTRL_TE0;
		irq  |= SAA7134_IRQ1_INTE_RA0_1 |
			SAA7134_IRQ1_INTE_RA0_0;
		cap = dev->video_q.curr->vb.field;
	}

	/* video capture -- dma 1+2 (planar modes) */
	if (dev->video_q.curr &&
	    dev->video_q.curr->fmt->planar) {
		ctrl |= SAA7134_MAIN_CTRL_TE4 |
			SAA7134_MAIN_CTRL_TE5;
	}

	/* screen overlay -- dma 0 + video task B */
	if (dev->ovenable) {
		task |= 0x10;
		ctrl |= SAA7134_MAIN_CTRL_TE1;
		ov = dev->ovfield;
	}

	/* vbi capture -- dma 0 + vbi task A+B */
	if (dev->vbi_q.curr) {
		task |= 0x22;
		ctrl |= SAA7134_MAIN_CTRL_TE2 |
			SAA7134_MAIN_CTRL_TE3;
		irq  |= SAA7134_IRQ1_INTE_RA0_7 |
			SAA7134_IRQ1_INTE_RA0_6 |
			SAA7134_IRQ1_INTE_RA0_5 |
			SAA7134_IRQ1_INTE_RA0_4;
	}

	/* audio capture -- dma 3 */
	if (dev->dmasound.dma_running) {
		ctrl |= SAA7134_MAIN_CTRL_TE6;
		irq  |= SAA7134_IRQ1_INTE_RA3_1 |
			SAA7134_IRQ1_INTE_RA3_0;
	}

	/* TS capture -- dma 5 */
	if (dev->ts_q.curr) {
		ctrl |= SAA7134_MAIN_CTRL_TE5;
		irq  |= SAA7134_IRQ1_INTE_RA2_1 |
			SAA7134_IRQ1_INTE_RA2_0;
	}

	/* set task conditions + field handling */
	if (V4L2_FIELD_HAS_BOTH(cap) || V4L2_FIELD_HAS_BOTH(ov) || cap == ov) {
		/* default config -- use full frames */
		saa_writeb(SAA7134_TASK_CONDITIONS(TASK_A), 0x0d);
		saa_writeb(SAA7134_TASK_CONDITIONS(TASK_B), 0x0d);
		saa_writeb(SAA7134_FIELD_HANDLING(TASK_A),  0x02);
		saa_writeb(SAA7134_FIELD_HANDLING(TASK_B),  0x02);
		split = 0;
	} else {
		/* split fields between tasks */
		if (V4L2_FIELD_TOP == cap) {
			/* odd A, even B, repeat */
			saa_writeb(SAA7134_TASK_CONDITIONS(TASK_A), 0x0d);
			saa_writeb(SAA7134_TASK_CONDITIONS(TASK_B), 0x0e);
		} else {
			/* odd B, even A, repeat */
			saa_writeb(SAA7134_TASK_CONDITIONS(TASK_A), 0x0e);
			saa_writeb(SAA7134_TASK_CONDITIONS(TASK_B), 0x0d);
		}
		saa_writeb(SAA7134_FIELD_HANDLING(TASK_A),  0x01);
		saa_writeb(SAA7134_FIELD_HANDLING(TASK_B),  0x01);
		split = 1;
	}

	/* irqs */
	saa_writeb(SAA7134_REGION_ENABLE, task);
	saa_writel(SAA7134_IRQ1,          irq);
	saa_andorl(SAA7134_MAIN_CTRL,
		   SAA7134_MAIN_CTRL_TE0 |
		   SAA7134_MAIN_CTRL_TE1 |
		   SAA7134_MAIN_CTRL_TE2 |
		   SAA7134_MAIN_CTRL_TE3 |
		   SAA7134_MAIN_CTRL_TE4 |
		   SAA7134_MAIN_CTRL_TE5 |
		   SAA7134_MAIN_CTRL_TE6,
		   ctrl);
	dprintk("dmabits: task=0x%02x ctrl=0x%02x irq=0x%x split=%s\n",
		task, ctrl, irq, split ? "no" : "yes");

	return 0;
}

/* ------------------------------------------------------------------ */
/* IRQ handler + helpers                                              */

static char *irqbits[] = {
	"DONE_RA0", "DONE_RA1", "DONE_RA2", "DONE_RA3",
	"AR", "PE", "PWR_ON", "RDCAP", "INTL", "FIDT", "MMC",
	"TRIG_ERR", "CONF_ERR", "LOAD_ERR",
	"GPIO16", "GPIO18", "GPIO22", "GPIO23"
};
#define IRQBITS ARRAY_SIZE(irqbits)

static void print_irqstatus(struct saa7134_dev *dev, int loop,
			    unsigned long report, unsigned long status)
{
	unsigned int i;

	printk(KERN_DEBUG "%s/irq[%d,%ld]: r=0x%lx s=0x%02lx",
	       dev->name,loop,jiffies,report,status);
	for (i = 0; i < IRQBITS; i++) {
		if (!(report & (1 << i)))
			continue;
		printk(" %s",irqbits[i]);
	}
	if (report & SAA7134_IRQ_REPORT_DONE_RA0) {
		printk(" | RA0=%s,%s,%s,%ld",
		       (status & 0x40) ? "vbi"  : "video",
		       (status & 0x20) ? "b"    : "a",
		       (status & 0x10) ? "odd"  : "even",
		       (status & 0x0f));
	}
	printk("\n");
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
static irqreturn_t saa7134_irq(int irq, void *dev_id, struct pt_regs *regs)
#else
static irqreturn_t saa7134_irq(int irq, void *dev_id)
#endif
{
	struct saa7134_dev *dev = (struct saa7134_dev*) dev_id;
	unsigned long report,status;
	int loop, handled = 0;

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 23)
	/* To make sure we see correct value of dev->insuspend */
	smp_rmb();
#endif
	if (dev->insuspend)
		goto out;

	for (loop = 0; loop < 10; loop++) {
		report = saa_readl(SAA7134_IRQ_REPORT);
		status = saa_readl(SAA7134_IRQ_STATUS);

		/* If dmasound support is active and we get a sound report,
		 * mask out the report and let the saa7134-alsa module deal
		 * with it */
		if ((report & SAA7134_IRQ_REPORT_DONE_RA3) &&
			(dev->dmasound.priv_data != NULL) )
		{
			if (irq_debug > 1)
				printk(KERN_DEBUG "%s/irq: preserving DMA sound interrupt\n",
				       dev->name);
			report &= ~SAA7134_IRQ_REPORT_DONE_RA3;
		}

		if (0 == report) {
			if (irq_debug > 1)
				printk(KERN_DEBUG "%s/irq: no (more) work\n",
				       dev->name);
			goto out;
		}

		handled = 1;
		saa_writel(SAA7134_IRQ_REPORT,report);
		if (irq_debug)
			print_irqstatus(dev,loop,report,status);

#if 0
		if (report & SAA7134_IRQ_REPORT_CONF_ERR)
			dump_statusregs(dev);
#endif

		if ((report & SAA7134_IRQ_REPORT_RDCAP) ||
			(report & SAA7134_IRQ_REPORT_INTL))
				saa7134_irq_video_signalchange(dev);


		if ((report & SAA7134_IRQ_REPORT_DONE_RA0) &&
		    (status & 0x60) == 0)
			saa7134_irq_video_done(dev,status);

		if ((report & SAA7134_IRQ_REPORT_DONE_RA0) &&
		    (status & 0x40) == 0x40)
			saa7134_irq_vbi_done(dev,status);

		if ((report & SAA7134_IRQ_REPORT_DONE_RA2) &&
		    card_has_mpeg(dev))
			saa7134_irq_ts_done(dev,status);

		if (report & SAA7134_IRQ_REPORT_GPIO16) {
			switch (dev->has_remote) {
				case SAA7134_REMOTE_GPIO:
					if (!dev->remote)
						break;
					if  (dev->remote->mask_keydown & 0x10000) {
						saa7134_input_irq(dev);
					}
					break;

				case SAA7134_REMOTE_I2C:
					break;			/* FIXME: invoke I2C get_key() */

				default:			/* GPIO16 not used by IR remote */
					break;
			}
		}

		if (report & SAA7134_IRQ_REPORT_GPIO18) {
			switch (dev->has_remote) {
				case SAA7134_REMOTE_GPIO:
					if (!dev->remote)
						break;
					if ((dev->remote->mask_keydown & 0x40000) ||
					    (dev->remote->mask_keyup & 0x40000)) {
						saa7134_input_irq(dev);
					}
					break;

				case SAA7134_REMOTE_I2C:
					break;			/* FIXME: invoke I2C get_key() */

				default:			/* GPIO18 not used by IR remote */
					break;
			}
		}
	}

	if (10 == loop) {
		print_irqstatus(dev,loop,report,status);
		if (report & SAA7134_IRQ_REPORT_PE) {
			/* disable all parity error */
			printk(KERN_WARNING "%s/irq: looping -- "
			       "clearing PE (parity error!) enable bit\n",dev->name);
			saa_clearl(SAA7134_IRQ2,SAA7134_IRQ2_INTE_PE);
		} else if (report & SAA7134_IRQ_REPORT_GPIO16) {
			/* disable gpio16 IRQ */
			printk(KERN_WARNING "%s/irq: looping -- "
			       "clearing GPIO16 enable bit\n",dev->name);
			saa_clearl(SAA7134_IRQ2, SAA7134_IRQ2_INTE_GPIO16_P);
			saa_clearl(SAA7134_IRQ2, SAA7134_IRQ2_INTE_GPIO16_N);
		} else if (report & SAA7134_IRQ_REPORT_GPIO18) {
			/* disable gpio18 IRQs */
			printk(KERN_WARNING "%s/irq: looping -- "
			       "clearing GPIO18 enable bit\n",dev->name);
			saa_clearl(SAA7134_IRQ2, SAA7134_IRQ2_INTE_GPIO18_P);
			saa_clearl(SAA7134_IRQ2, SAA7134_IRQ2_INTE_GPIO18_N);
		} else {
			/* disable all irqs */
			printk(KERN_WARNING "%s/irq: looping -- "
			       "clearing all enable bits\n",dev->name);
			saa_writel(SAA7134_IRQ1,0);
			saa_writel(SAA7134_IRQ2,0);
		}
	}

 out:
	return IRQ_RETVAL(handled);
}

/* ------------------------------------------------------------------ */

/* early init (no i2c, no irq) */

static int saa7134_hw_enable1(struct saa7134_dev *dev)
{
	/* RAM FIFO config */
	saa_writel(SAA7134_FIFO_SIZE, 0x08070503);
	saa_writel(SAA7134_THRESHOULD, 0x02020202);

	/* enable audio + video processing */
	saa_writel(SAA7134_MAIN_CTRL,
			SAA7134_MAIN_CTRL_VPLLE |
			SAA7134_MAIN_CTRL_APLLE |
			SAA7134_MAIN_CTRL_EXOSC |
			SAA7134_MAIN_CTRL_EVFE1 |
			SAA7134_MAIN_CTRL_EVFE2 |
			SAA7134_MAIN_CTRL_ESFE  |
			SAA7134_MAIN_CTRL_EBDAC);

	/*
	* Initialize OSS _after_ enabling audio clock PLL and audio processing.
	* OSS initialization writes to registers via the audio DSP; these
	* writes will fail unless the audio clock has been started.  At worst,
	* audio will not work.
	*/

	/* enable peripheral devices */
	saa_writeb(SAA7134_SPECIAL_MODE, 0x01);

	/* set vertical line numbering start (vbi needs this) */
	saa_writeb(SAA7134_SOURCE_TIMING2, 0x20);

	return 0;
}

static int saa7134_hwinit1(struct saa7134_dev *dev)
{
	dprintk("hwinit1\n");

	saa_writel(SAA7134_IRQ1, 0);
	saa_writel(SAA7134_IRQ2, 0);

	/* Clear any stale IRQ reports */
	saa_writel(SAA7134_IRQ_REPORT, saa_readl(SAA7134_IRQ_REPORT));

	mutex_init(&dev->lock);
	spin_lock_init(&dev->slock);

	saa7134_track_gpio(dev,"pre-init");
	saa7134_video_init1(dev);
	saa7134_vbi_init1(dev);
	if (card_has_mpeg(dev))
		saa7134_ts_init1(dev);
	saa7134_input_init1(dev);

	saa7134_hw_enable1(dev);

	return 0;
}

/* late init (with i2c + irq) */
static int saa7134_hw_enable2(struct saa7134_dev *dev)
{

	unsigned int irq2_mask;

	/* enable IRQ's */
	irq2_mask =
		SAA7134_IRQ2_INTE_DEC3    |
		SAA7134_IRQ2_INTE_DEC2    |
		SAA7134_IRQ2_INTE_DEC1    |
		SAA7134_IRQ2_INTE_DEC0    |
		SAA7134_IRQ2_INTE_PE      |
		SAA7134_IRQ2_INTE_AR;

	if (dev->has_remote == SAA7134_REMOTE_GPIO && dev->remote) {
		if (dev->remote->mask_keydown & 0x10000)
			irq2_mask |= SAA7134_IRQ2_INTE_GPIO16_N;
		else {		/* Allow enabling both IRQ edge triggers */
			if (dev->remote->mask_keydown & 0x40000)
				irq2_mask |= SAA7134_IRQ2_INTE_GPIO18_P;
			if (dev->remote->mask_keyup & 0x40000)
				irq2_mask |= SAA7134_IRQ2_INTE_GPIO18_N;
		}
	}

	if (dev->has_remote == SAA7134_REMOTE_I2C) {
		request_module("ir-kbd-i2c");
	}

	saa_writel(SAA7134_IRQ1, 0);
	saa_writel(SAA7134_IRQ2, irq2_mask);

	return 0;
}

static int saa7134_hwinit2(struct saa7134_dev *dev)
{

	dprintk("hwinit2\n");

	saa7134_video_init2(dev);
	saa7134_tvaudio_init2(dev);

	saa7134_hw_enable2(dev);

	return 0;
}


/* shutdown */
static int saa7134_hwfini(struct saa7134_dev *dev)
{
	dprintk("hwfini\n");

	if (card_has_mpeg(dev))
		saa7134_ts_fini(dev);
	saa7134_input_fini(dev);
	saa7134_vbi_fini(dev);
	saa7134_tvaudio_fini(dev);
	return 0;
}

static void __devinit must_configure_manually(void)
{
	unsigned int i,p;

	printk(KERN_WARNING
	       "saa7134: <rant>\n"
	       "saa7134:  Congratulations!  Your TV card vendor saved a few\n"
	       "saa7134:  cents for a eeprom, thus your pci board has no\n"
	       "saa7134:  subsystem ID and I can't identify it automatically\n"
	       "saa7134: </rant>\n"
	       "saa7134: I feel better now.  Ok, here are the good news:\n"
	       "saa7134: You can use the card=<nr> insmod option to specify\n"
	       "saa7134: which board do you have.  The list:\n");
	for (i = 0; i < saa7134_bcount; i++) {
		printk(KERN_WARNING "saa7134:   card=%d -> %-40.40s",
		       i,saa7134_boards[i].name);
		for (p = 0; saa7134_pci_tbl[p].driver_data; p++) {
			if (saa7134_pci_tbl[p].driver_data != i)
				continue;
			printk(" %04x:%04x",
			       saa7134_pci_tbl[p].subvendor,
			       saa7134_pci_tbl[p].subdevice);
		}
		printk("\n");
	}
}

static struct video_device *vdev_init(struct saa7134_dev *dev,
				      struct video_device *template,
				      char *type)
{
	struct video_device *vfd;

	vfd = video_device_alloc();
	if (NULL == vfd)
		return NULL;
	*vfd = *template;
	vfd->v4l2_dev  = &dev->v4l2_dev;
	vfd->release = video_device_release;
	vfd->debug   = video_debug;
	snprintf(vfd->name, sizeof(vfd->name), "%s %s (%s)",
		 dev->name, type, saa7134_boards[dev->board].name);
	video_set_drvdata(vfd, dev);
	return vfd;
}

static void saa7134_unregister_video(struct saa7134_dev *dev)
{
	if (dev->video_dev) {
		if (video_is_registered(dev->video_dev))
			video_unregister_device(dev->video_dev);
		else
			video_device_release(dev->video_dev);
		dev->video_dev = NULL;
	}
	if (dev->vbi_dev) {
		if (video_is_registered(dev->vbi_dev))
			video_unregister_device(dev->vbi_dev);
		else
			video_device_release(dev->vbi_dev);
		dev->vbi_dev = NULL;
	}
	if (dev->radio_dev) {
		if (video_is_registered(dev->radio_dev))
			video_unregister_device(dev->radio_dev);
		else
			video_device_release(dev->radio_dev);
		dev->radio_dev = NULL;
	}
}

static void mpeg_ops_attach(struct saa7134_mpeg_ops *ops,
			    struct saa7134_dev *dev)
{
	int err;

	if (NULL != dev->mops)
		return;
	if (saa7134_boards[dev->board].mpeg != ops->type)
		return;
	err = ops->init(dev);
	if (0 != err)
		return;
	dev->mops = ops;
}

static void mpeg_ops_detach(struct saa7134_mpeg_ops *ops,
			    struct saa7134_dev *dev)
{
	if (NULL == dev->mops)
		return;
	if (dev->mops != ops)
		return;
	dev->mops->fini(dev);
	dev->mops = NULL;
}

static int __devinit saa7134_initdev(struct pci_dev *pci_dev,
				     const struct pci_device_id *pci_id)
{
	struct saa7134_dev *dev;
	struct saa7134_mpeg_ops *mops;
	int err;

	if (saa7134_devcount == SAA7134_MAXBOARDS)
		return -ENOMEM;

	dev = kzalloc(sizeof(*dev),GFP_KERNEL);
	if (NULL == dev)
		return -ENOMEM;

	err = v4l2_device_register(&pci_dev->dev, &dev->v4l2_dev);
	if (err)
		goto fail0;

	/* pci init */
	dev->pci = pci_dev;
	if (pci_enable_device(pci_dev)) {
		err = -EIO;
		goto fail1;
	}

	dev->nr = saa7134_devcount;
	sprintf(dev->name,"saa%x[%d]",pci_dev->device,dev->nr);

	/* pci quirks */
	if (pci_pci_problems) {
		if (pci_pci_problems & PCIPCI_TRITON)
			printk(KERN_INFO "%s: quirk: PCIPCI_TRITON\n", dev->name);
		if (pci_pci_problems & PCIPCI_NATOMA)
			printk(KERN_INFO "%s: quirk: PCIPCI_NATOMA\n", dev->name);
		if (pci_pci_problems & PCIPCI_VIAETBF)
			printk(KERN_INFO "%s: quirk: PCIPCI_VIAETBF\n", dev->name);
		if (pci_pci_problems & PCIPCI_VSFX)
			printk(KERN_INFO "%s: quirk: PCIPCI_VSFX\n",dev->name);
#ifdef PCIPCI_ALIMAGIK
		if (pci_pci_problems & PCIPCI_ALIMAGIK) {
			printk(KERN_INFO "%s: quirk: PCIPCI_ALIMAGIK -- latency fixup\n",
			       dev->name);
			latency = 0x0A;
		}
#endif
		if (pci_pci_problems & (PCIPCI_FAIL|PCIAGP_FAIL)) {
			printk(KERN_INFO "%s: quirk: this driver and your "
					"chipset may not work together"
					" in overlay mode.\n",dev->name);
			if (!saa7134_no_overlay) {
				printk(KERN_INFO "%s: quirk: overlay "
						"mode will be disabled.\n",
						dev->name);
				saa7134_no_overlay = 1;
			} else {
				printk(KERN_INFO "%s: quirk: overlay "
						"mode will be forced. Use this"
						" option at your own risk.\n",
						dev->name);
			}
		}
	}
	if (UNSET != latency) {
		printk(KERN_INFO "%s: setting pci latency timer to %d\n",
		       dev->name,latency);
		pci_write_config_byte(pci_dev, PCI_LATENCY_TIMER, latency);
	}

	/* print pci info */
	pci_read_config_byte(pci_dev, PCI_CLASS_REVISION, &dev->pci_rev);
	pci_read_config_byte(pci_dev, PCI_LATENCY_TIMER,  &dev->pci_lat);
	printk(KERN_INFO "%s: found at %s, rev: %d, irq: %d, "
	       "latency: %d, mmio: 0x%llx\n", dev->name,
	       pci_name(pci_dev), dev->pci_rev, pci_dev->irq,
	       dev->pci_lat,(unsigned long long)pci_resource_start(pci_dev,0));
	pci_set_master(pci_dev);
	if (!pci_dma_supported(pci_dev, DMA_BIT_MASK(32))) {
		printk("%s: Oops: no 32bit PCI DMA ???\n",dev->name);
		err = -EIO;
		goto fail1;
	}

	/* board config */
	dev->board = pci_id->driver_data;
	if (card[dev->nr] >= 0 &&
	    card[dev->nr] < saa7134_bcount)
		dev->board = card[dev->nr];
	if (SAA7134_BOARD_NOAUTO == dev->board) {
		must_configure_manually();
		dev->board = SAA7134_BOARD_UNKNOWN;
	}
	dev->autodetected = card[dev->nr] != dev->board;
	dev->tuner_type = saa7134_boards[dev->board].tuner_type;
	dev->tuner_addr = saa7134_boards[dev->board].tuner_addr;
	dev->radio_type = saa7134_boards[dev->board].radio_type;
	dev->radio_addr = saa7134_boards[dev->board].radio_addr;
	dev->tda9887_conf = saa7134_boards[dev->board].tda9887_conf;
	if (UNSET != tuner[dev->nr])
		dev->tuner_type = tuner[dev->nr];
	printk(KERN_INFO "%s: subsystem: %04x:%04x, board: %s [card=%d,%s]\n",
		dev->name,pci_dev->subsystem_vendor,
		pci_dev->subsystem_device,saa7134_boards[dev->board].name,
		dev->board, dev->autodetected ?
		"autodetected" : "insmod option");

	/* get mmio */
	if (!request_mem_region(pci_resource_start(pci_dev,0),
				pci_resource_len(pci_dev,0),
				dev->name)) {
		err = -EBUSY;
		printk(KERN_ERR "%s: can't get MMIO memory @ 0x%llx\n",
		       dev->name,(unsigned long long)pci_resource_start(pci_dev,0));
		goto fail1;
	}
	dev->lmmio = ioremap(pci_resource_start(pci_dev, 0),
			     pci_resource_len(pci_dev, 0));
	dev->bmmio = (__u8 __iomem *)dev->lmmio;
	if (NULL == dev->lmmio) {
		err = -EIO;
		printk(KERN_ERR "%s: can't ioremap() MMIO memory\n",
		       dev->name);
		goto fail2;
	}

	/* initialize hardware #1 */
	saa7134_board_init1(dev);
	saa7134_hwinit1(dev);

	/* get irq */
	err = request_irq(pci_dev->irq, saa7134_irq,
			  IRQF_SHARED | IRQF_DISABLED, dev->name, dev);
	if (err < 0) {
		printk(KERN_ERR "%s: can't get IRQ %d\n",
		       dev->name,pci_dev->irq);
		goto fail3;
	}

	/* wait a bit, register i2c bus */
	msleep(100);
	saa7134_i2c_register(dev);
	saa7134_board_init2(dev);

	saa7134_hwinit2(dev);

	/* load i2c helpers */
	if (card_is_empress(dev)) {
		struct v4l2_subdev *sd =
			v4l2_i2c_new_subdev(&dev->v4l2_dev, &dev->i2c_adap,
				"saa6752hs", "saa6752hs",
				saa7134_boards[dev->board].empress_addr, NULL);

		if (sd)
			sd->grp_id = GRP_EMPRESS;
	}

	if (saa7134_boards[dev->board].rds_addr) {
		struct v4l2_subdev *sd;

		sd = v4l2_i2c_new_subdev(&dev->v4l2_dev,
				&dev->i2c_adap,	"saa6588", "saa6588",
				0, I2C_ADDRS(saa7134_boards[dev->board].rds_addr));
		if (sd) {
			printk(KERN_INFO "%s: found RDS decoder\n", dev->name);
			dev->has_rds = 1;
		}
	}

	request_submodules(dev);

	v4l2_prio_init(&dev->prio);

	mutex_lock(&saa7134_devlist_lock);
	list_for_each_entry(mops, &mops_list, next)
		mpeg_ops_attach(mops, dev);
	list_add_tail(&dev->devlist, &saa7134_devlist);
	mutex_unlock(&saa7134_devlist_lock);

	/* check for signal */
	saa7134_irq_video_signalchange(dev);

	if (TUNER_ABSENT != dev->tuner_type)
		saa_call_all(dev, core, s_power, 0);

	/* register v4l devices */
	if (saa7134_no_overlay > 0)
		printk(KERN_INFO "%s: Overlay support disabled.\n", dev->name);

	dev->video_dev = vdev_init(dev,&saa7134_video_template,"video");
	err = video_register_device(dev->video_dev,VFL_TYPE_GRABBER,
				    video_nr[dev->nr]);
	if (err < 0) {
		printk(KERN_INFO "%s: can't register video device\n",
		       dev->name);
		goto fail4;
	}
	printk(KERN_INFO "%s: registered device %s [v4l2]\n",
	       dev->name, video_device_node_name(dev->video_dev));

	dev->vbi_dev = vdev_init(dev, &saa7134_video_template, "vbi");

	err = video_register_device(dev->vbi_dev,VFL_TYPE_VBI,
				    vbi_nr[dev->nr]);
	if (err < 0)
		goto fail4;
	printk(KERN_INFO "%s: registered device %s\n",
	       dev->name, video_device_node_name(dev->vbi_dev));

	if (card_has_radio(dev)) {
		dev->radio_dev = vdev_init(dev,&saa7134_radio_template,"radio");
		err = video_register_device(dev->radio_dev,VFL_TYPE_RADIO,
					    radio_nr[dev->nr]);
		if (err < 0)
			goto fail4;
		printk(KERN_INFO "%s: registered device %s\n",
		       dev->name, video_device_node_name(dev->radio_dev));
	}

	/* everything worked */
	saa7134_devcount++;

	if (saa7134_dmasound_init && !dev->dmasound.priv_data)
		saa7134_dmasound_init(dev);

	return 0;

 fail4:
	saa7134_unregister_video(dev);
	saa7134_i2c_unregister(dev);
	free_irq(pci_dev->irq, dev);
 fail3:
	saa7134_hwfini(dev);
	iounmap(dev->lmmio);
 fail2:
	release_mem_region(pci_resource_start(pci_dev,0),
			   pci_resource_len(pci_dev,0));
 fail1:
	v4l2_device_unregister(&dev->v4l2_dev);
 fail0:
	kfree(dev);
	return err;
}

static void __devexit saa7134_finidev(struct pci_dev *pci_dev)
{
	struct v4l2_device *v4l2_dev = pci_get_drvdata(pci_dev);
	struct saa7134_dev *dev = container_of(v4l2_dev, struct saa7134_dev, v4l2_dev);
	struct saa7134_mpeg_ops *mops;

	/* Release DMA sound modules if present */
	if (saa7134_dmasound_exit && dev->dmasound.priv_data) {
		saa7134_dmasound_exit(dev);
	}

	/* debugging ... */
	if (irq_debug) {
		u32 report = saa_readl(SAA7134_IRQ_REPORT);
		u32 status = saa_readl(SAA7134_IRQ_STATUS);
		print_irqstatus(dev,42,report,status);
	}

	/* disable peripheral devices */
	saa_writeb(SAA7134_SPECIAL_MODE,0);

	/* shutdown hardware */
	saa_writel(SAA7134_IRQ1,0);
	saa_writel(SAA7134_IRQ2,0);
	saa_writel(SAA7134_MAIN_CTRL,0);

	/* shutdown subsystems */
	saa7134_hwfini(dev);

	/* unregister */
	mutex_lock(&saa7134_devlist_lock);
	list_del(&dev->devlist);
	list_for_each_entry(mops, &mops_list, next)
		mpeg_ops_detach(mops, dev);
	mutex_unlock(&saa7134_devlist_lock);
	saa7134_devcount--;

	saa7134_i2c_unregister(dev);
	saa7134_unregister_video(dev);


	/* the DMA sound modules should be unloaded before reaching
	   this, but just in case they are still present... */
	if (dev->dmasound.priv_data != NULL) {
		free_irq(pci_dev->irq, &dev->dmasound);
		dev->dmasound.priv_data = NULL;
	}


	/* release resources */
	free_irq(pci_dev->irq, dev);
	iounmap(dev->lmmio);
	release_mem_region(pci_resource_start(pci_dev,0),
			   pci_resource_len(pci_dev,0));

#if 0  /* causes some trouble when reinserting the driver ... */
	pci_disable_device(pci_dev);
#endif

	v4l2_device_unregister(&dev->v4l2_dev);

	/* free memory */
	kfree(dev);
}

#ifdef CONFIG_PM

/* resends a current buffer in queue after resume */
static int saa7134_buffer_requeue(struct saa7134_dev *dev,
				  struct saa7134_dmaqueue *q)
{
	struct saa7134_buf *buf, *next;

	assert_spin_locked(&dev->slock);

	buf  = q->curr;
	next = buf;
	dprintk("buffer_requeue\n");

	if (!buf)
		return 0;

	dprintk("buffer_requeue : resending active buffers \n");

	if (!list_empty(&q->queue))
		next = list_entry(q->queue.next, struct saa7134_buf,
					  vb.queue);
	buf->activate(dev, buf, next);

	return 0;
}

static int saa7134_suspend(struct pci_dev *pci_dev , pm_message_t state)
{
	struct v4l2_device *v4l2_dev = pci_get_drvdata(pci_dev);
	struct saa7134_dev *dev = container_of(v4l2_dev, struct saa7134_dev, v4l2_dev);

	/* disable overlay - apps should enable it explicitly on resume*/
	dev->ovenable = 0;

	/* Disable interrupts, DMA, and rest of the chip*/
	saa_writel(SAA7134_IRQ1, 0);
	saa_writel(SAA7134_IRQ2, 0);
	saa_writel(SAA7134_MAIN_CTRL, 0);

	dev->insuspend = 1;
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 23)
	smp_wmb();
#endif
	synchronize_irq(pci_dev->irq);

	/* ACK interrupts once more, just in case,
		since the IRQ handler won't ack them anymore*/

	saa_writel(SAA7134_IRQ_REPORT, saa_readl(SAA7134_IRQ_REPORT));

	/* Disable timeout timers - if we have active buffers, we will
	   fill them on resume*/

	del_timer(&dev->video_q.timeout);
	del_timer(&dev->vbi_q.timeout);
	del_timer(&dev->ts_q.timeout);

	if (dev->remote)
		saa7134_ir_stop(dev);

	pci_save_state(pci_dev);
	pci_set_power_state(pci_dev, pci_choose_state(pci_dev, state));

	return 0;
}

static int saa7134_resume(struct pci_dev *pci_dev)
{
	struct v4l2_device *v4l2_dev = pci_get_drvdata(pci_dev);
	struct saa7134_dev *dev = container_of(v4l2_dev, struct saa7134_dev, v4l2_dev);
	unsigned long flags;

	pci_set_power_state(pci_dev, PCI_D0);
	pci_restore_state(pci_dev);

	/* Do things that are done in saa7134_initdev ,
		except of initializing memory structures.*/

	saa7134_board_init1(dev);

	/* saa7134_hwinit1 */
	if (saa7134_boards[dev->board].video_out)
		saa7134_videoport_init(dev);
	if (card_has_mpeg(dev))
		saa7134_ts_init_hw(dev);
	if (dev->remote)
		saa7134_ir_start(dev);
	saa7134_hw_enable1(dev);

	msleep(100);

	saa7134_board_init2(dev);

	/*saa7134_hwinit2*/
	saa7134_set_tvnorm_hw(dev);
	saa7134_tvaudio_setmute(dev);
	saa7134_tvaudio_setvolume(dev, dev->ctl_volume);
	saa7134_tvaudio_init(dev);
	saa7134_tvaudio_do_scan(dev);
	saa7134_enable_i2s(dev);
	saa7134_hw_enable2(dev);

	saa7134_irq_video_signalchange(dev);

	/*resume unfinished buffer(s)*/
	spin_lock_irqsave(&dev->slock, flags);
	saa7134_buffer_requeue(dev, &dev->video_q);
	saa7134_buffer_requeue(dev, &dev->vbi_q);
	saa7134_buffer_requeue(dev, &dev->ts_q);

	/* FIXME: Disable DMA audio sound - temporary till proper support
		  is implemented*/

	dev->dmasound.dma_running = 0;

	/* start DMA now*/
	dev->insuspend = 0;
	smp_wmb();
	saa7134_set_dmabits(dev);
	spin_unlock_irqrestore(&dev->slock, flags);

	return 0;
}
#endif

/* ----------------------------------------------------------- */

int saa7134_ts_register(struct saa7134_mpeg_ops *ops)
{
	struct saa7134_dev *dev;

	mutex_lock(&saa7134_devlist_lock);
	list_for_each_entry(dev, &saa7134_devlist, devlist)
		mpeg_ops_attach(ops, dev);
	list_add_tail(&ops->next,&mops_list);
	mutex_unlock(&saa7134_devlist_lock);
	return 0;
}

void saa7134_ts_unregister(struct saa7134_mpeg_ops *ops)
{
	struct saa7134_dev *dev;

	mutex_lock(&saa7134_devlist_lock);
	list_del(&ops->next);
	list_for_each_entry(dev, &saa7134_devlist, devlist)
		mpeg_ops_detach(ops, dev);
	mutex_unlock(&saa7134_devlist_lock);
}

EXPORT_SYMBOL(saa7134_ts_register);
EXPORT_SYMBOL(saa7134_ts_unregister);

/* ----------------------------------------------------------- */

static struct pci_driver saa7134_pci_driver = {
	.name     = "saa7134",
	.id_table = saa7134_pci_tbl,
	.probe    = saa7134_initdev,
	.remove   = __devexit_p(saa7134_finidev),
#ifdef CONFIG_PM
	.suspend  = saa7134_suspend,
	.resume   = saa7134_resume
#endif
};

static int __init saa7134_init(void)
{
	INIT_LIST_HEAD(&saa7134_devlist);
	printk(KERN_INFO "saa7130/34: v4l2 driver version %d.%d.%d loaded\n",
	       (SAA7134_VERSION_CODE >> 16) & 0xff,
	       (SAA7134_VERSION_CODE >>  8) & 0xff,
	       SAA7134_VERSION_CODE & 0xff);
#ifdef SNAPSHOT
	printk(KERN_INFO "saa7130/34: snapshot date %04d-%02d-%02d\n",
	       SNAPSHOT/10000, (SNAPSHOT/100)%100, SNAPSHOT%100);
#endif
	return pci_register_driver(&saa7134_pci_driver);
}

static void __exit saa7134_fini(void)
{
	pci_unregister_driver(&saa7134_pci_driver);
}

module_init(saa7134_init);
module_exit(saa7134_fini);

/* ----------------------------------------------------------- */

EXPORT_SYMBOL(saa7134_set_gpio);
EXPORT_SYMBOL(saa7134_boards);

/* ----------------- for the DMA sound modules --------------- */

EXPORT_SYMBOL(saa7134_dmasound_init);
EXPORT_SYMBOL(saa7134_dmasound_exit);
EXPORT_SYMBOL(saa7134_pgtable_free);
EXPORT_SYMBOL(saa7134_pgtable_build);
EXPORT_SYMBOL(saa7134_pgtable_alloc);
EXPORT_SYMBOL(saa7134_set_dmabits);

/* ----------------------------------------------------------- */
/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
