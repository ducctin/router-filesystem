/*
 * vp27smpx - driver version 0.0.1
 *
 * Copyright (C) 2007 Hans Verkuil <hverkuil@xs4all.nl>
 *
 * Based on a tvaudio patch from Takahiro Adachi <tadachi@tadachi-net.com>
 * and Kazuhiko Kawakami <kazz-0@mail.goo.ne.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>
#include <linux/i2c.h>
#include <linux/i2c-id.h>
#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-i2c-drv.h>
#include "compat.h"

MODULE_DESCRIPTION("vp27smpx driver");
MODULE_AUTHOR("Hans Verkuil");
MODULE_LICENSE("GPL");

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 26)
static unsigned short normal_i2c[] = { 0xb6 >> 1, I2C_CLIENT_END };

I2C_CLIENT_INSMOD;
#endif

/* ----------------------------------------------------------------------- */

struct vp27smpx_state {
	struct v4l2_subdev sd;
	int radio;
	u32 audmode;
};

static inline struct vp27smpx_state *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct vp27smpx_state, sd);
}

static void vp27smpx_set_audmode(struct v4l2_subdev *sd, u32 audmode)
{
	struct vp27smpx_state *state = to_state(sd);
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	u8 data[3] = { 0x00, 0x00, 0x04 };

	switch (audmode) {
	case V4L2_TUNER_MODE_MONO:
	case V4L2_TUNER_MODE_LANG1:
		break;
	case V4L2_TUNER_MODE_STEREO:
	case V4L2_TUNER_MODE_LANG1_LANG2:
		data[1] = 0x01;
		break;
	case V4L2_TUNER_MODE_LANG2:
		data[1] = 0x02;
		break;
	}

	if (i2c_master_send(client, data, sizeof(data)) != sizeof(data))
		v4l2_err(sd, "I/O error setting audmode\n");
	else
		state->audmode = audmode;
}

static int vp27smpx_s_radio(struct v4l2_subdev *sd)
{
	struct vp27smpx_state *state = to_state(sd);

	state->radio = 1;
	return 0;
}

static int vp27smpx_s_std(struct v4l2_subdev *sd, v4l2_std_id norm)
{
	struct vp27smpx_state *state = to_state(sd);

	state->radio = 0;
	return 0;
}

static int vp27smpx_s_tuner(struct v4l2_subdev *sd, struct v4l2_tuner *vt)
{
	struct vp27smpx_state *state = to_state(sd);

	if (!state->radio)
		vp27smpx_set_audmode(sd, vt->audmode);
	return 0;
}

static int vp27smpx_g_tuner(struct v4l2_subdev *sd, struct v4l2_tuner *vt)
{
	struct vp27smpx_state *state = to_state(sd);

	if (state->radio)
		return 0;
	vt->audmode = state->audmode;
	vt->capability = V4L2_TUNER_CAP_STEREO |
		V4L2_TUNER_CAP_LANG1 | V4L2_TUNER_CAP_LANG2;
	vt->rxsubchans = V4L2_TUNER_SUB_MONO;
	return 0;
}

static int vp27smpx_g_chip_ident(struct v4l2_subdev *sd, struct v4l2_dbg_chip_ident *chip)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	return v4l2_chip_ident_i2c_client(client, chip, V4L2_IDENT_VP27SMPX, 0);
}

static int vp27smpx_log_status(struct v4l2_subdev *sd)
{
	struct vp27smpx_state *state = to_state(sd);

	v4l2_info(sd, "Audio Mode: %u%s\n", state->audmode,
			state->radio ? " (Radio)" : "");
	return 0;
}

/* ----------------------------------------------------------------------- */

static const struct v4l2_subdev_core_ops vp27smpx_core_ops = {
	.log_status = vp27smpx_log_status,
	.g_chip_ident = vp27smpx_g_chip_ident,
	.s_std = vp27smpx_s_std,
};

static const struct v4l2_subdev_tuner_ops vp27smpx_tuner_ops = {
	.s_radio = vp27smpx_s_radio,
	.s_tuner = vp27smpx_s_tuner,
	.g_tuner = vp27smpx_g_tuner,
};

static const struct v4l2_subdev_ops vp27smpx_ops = {
	.core = &vp27smpx_core_ops,
	.tuner = &vp27smpx_tuner_ops,
};

/* ----------------------------------------------------------------------- */

/* i2c implementation */

/*
 * Generic i2c probe
 * concerning the addresses: i2c wants 7 bit (without the r/w bit), so '>>1'
 */

static int vp27smpx_probe(struct i2c_client *client,
			  const struct i2c_device_id *id)
{
	struct vp27smpx_state *state;
	struct v4l2_subdev *sd;

	/* Check if the adapter supports the needed features */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -EIO;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 26)
	snprintf(client->name, sizeof(client->name) - 1, "vp27smpx");
#endif
	v4l_info(client, "chip found @ 0x%x (%s)\n",
			client->addr << 1, client->adapter->name);

	state = kzalloc(sizeof(struct vp27smpx_state), GFP_KERNEL);
	if (state == NULL)
		return -ENOMEM;
	sd = &state->sd;
	v4l2_i2c_subdev_init(sd, client, &vp27smpx_ops);
	state->audmode = V4L2_TUNER_MODE_STEREO;

	/* initialize vp27smpx */
	vp27smpx_set_audmode(sd, state->audmode);
	return 0;
}

static int vp27smpx_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);

	v4l2_device_unregister_subdev(sd);
	kfree(to_state(sd));
	return 0;
}

/* ----------------------------------------------------------------------- */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 26)
static const struct i2c_device_id vp27smpx_id[] = {
	{ "vp27smpx", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, vp27smpx_id);
#endif

static struct v4l2_i2c_driver_data v4l2_i2c_data = {
	.name = "vp27smpx",
	.probe = vp27smpx_probe,
	.remove = vp27smpx_remove,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 26)
	.id_table = vp27smpx_id,
#endif
};
