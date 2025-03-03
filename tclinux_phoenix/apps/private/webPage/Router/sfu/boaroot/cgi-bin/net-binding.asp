<%
if Request_Form("formcommit") = "1" then
	TCWebApi_set("WebCurSet_Entry","VlanBind_id","entryidx")
	TCWebApi_set("VlanBind_Entry","Active","bindmode")
	if Request_Form("bindmode") = "Yes" then
		TCWebApi_set("VlanBind_Entry","mn","bindvalue")
	else
		TCWebApi_set("VlanBind_Entry","mn","empty")
	end if
	TCWebApi_commit("VlanBind_Entry")	
	tcWebApi_save()
end if
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD><TITLE>绑定设置</TITLE>
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=Content-Type content="text/html; charset=gb2312"><LINK 
href="/JS/stylemain.css" type=text/css rel=stylesheet>
<style type="text/css">
.title_01 {
	COLOR: #5c5d55
}
.prompt {
	PADDING-BOTTOM: 5px; PADDING-LEFT: 0px; PADDING-RIGHT: 0px; FONT-FAMILY: "Tohama", "Arial", "????"; FONT-SIZE: 12px; PADDING-TOP: 5px
}
</style>
<SCRIPT language=javascript src="/JS/menu.js"></SCRIPT>
<SCRIPT language=javascript src="/JS/util.js"></SCRIPT>
<SCRIPT language=JavaScript src="/JS/printtable.js"></SCRIPT>
<SCRIPT language=JavaScript type=text/javascript>
function LoadFrame()
{
}

function ModifyInstance(index)
{
  var lanmode = LanArray[index -1].Mode;
  var vlanpair = LanArray[index -1].Vlan;

  document.getElementById("TableUrlInfo").style.display = "block";
  getElById("UrlAddressControl").value = vlanpair;
  
  if (lanmode == 0)
  {
      getElById("ChooseDeviceType").value = "lanwanbinding";
      getElById("BindVlanRow").style.display = "none";        
  }
  else if (lanmode == 1)
  {
      getElById("ChooseDeviceType").value = "vlanbinding"; 
      getElById("BindVlanRow").style.display = "block";       
  }
}

 
function setControl(index) 
{ 
	var identi = "";

	selectIndex = index;
	if (index < -1)
		return;

	identi = index > 4 ? "SSID"+(index-4) : "LAN"+index;
	document.getElementById("PortId").innerHTML = identi;

	return ModifyInstance(index);
}



function BindInfoClass(Mode, Vlan)
{
    this.Mode = ( 'Yes' == Mode ) ? 1 : 0;
    this.Vlan = ( 'N/A' == Vlan ) ? '' : Vlan;
}

var LanArray = new Array(
new BindInfoClass('<% tcWebApi_get("VlanBind_Entry0","Active","s") %>', '<% tcWebApi_get("VlanBind_Entry0","mn","s") %>')
, new BindInfoClass('<% tcWebApi_get("VlanBind_Entry1","Active","s") %>', '<% tcWebApi_get("VlanBind_Entry1","mn","s") %>')
, new BindInfoClass('<% tcWebApi_get("VlanBind_Entry2","Active","s") %>', '<% tcWebApi_get("VlanBind_Entry2","mn","s") %>')
, new BindInfoClass('<% tcWebApi_get("VlanBind_Entry3","Active","s") %>', '<% tcWebApi_get("VlanBind_Entry3","mn","s") %>')
, new BindInfoClass('<% tcWebApi_get("VlanBind_Entry4","Active","s") %>', '<% tcWebApi_get("VlanBind_Entry4","mn","s") %>')
, new BindInfoClass('<% tcWebApi_get("VlanBind_Entry5","Active","s") %>', '<% tcWebApi_get("VlanBind_Entry5","mn","s") %>')
, new BindInfoClass('<% tcWebApi_get("VlanBind_Entry6","Active","s") %>', '<% tcWebApi_get("VlanBind_Entry6","mn","s") %>')
, new BindInfoClass('<% tcWebApi_get("VlanBind_Entry7","Active","s") %>', '<% tcWebApi_get("VlanBind_Entry7","mn","s") %>')
,null);


//创建port vlan 路由链表 
function CreateRouteList()
{     
  var HtmlCode = "";
  var Lan_Indenti1 = "LAN";
  var Lan_Indenti2 = "SSID";
  var Lan = "";
  var LanCnt = 4;
  var LanWire = 4;

	ssidNum = '<% tcWebApi_get("WLan_Common","BssidNum","s") %>';

	if ( isPlusInteger(ssidNum) )
		LanCnt += parseInt(ssidNum);
  for (var i = 1; i <= LanCnt; i++)
  {  
      var modestr = ""
      if (LanArray[i-1].Mode == 0)
      {
          modestr = "端口绑定";
      }
      else if (LanArray[i-1].Mode == 1)
      {
          modestr = "VLAN绑定";
      }

      Lan = i > LanWire ? (Lan_Indenti2+(i-4)) : (Lan_Indenti1+i);

      HtmlCode += '<tr id= "record_' + i +'" align = "center" class="table1_01" onclick="selectLine(this.id); " ';
      <% if TCWebApi_get("WebCustom_Entry","isCuc2PortSupported","h" ) = "Yes" then %>
      if(Lan == Lan_Indenti1 + 3 || Lan == Lan_Indenti1 + 4)
      	HtmlCode += 'style="display:none" ';
    	<% end if %>
    	<% if TCWebApi_get("WebCustom_Entry","isWLanSupported","h" ) <> "Yes" then %>
      if(Lan == Lan_Indenti2+(i-4))
      	HtmlCode += 'style="display:none" ';
    	<% end if %>
      HtmlCode += '>';
      HtmlCode += '<td >' + Lan + '</td>';

      HtmlCode += '<td>' +modestr + '</td>';
      
      if( (LanArray[i-1].Vlan == "") || (LanArray[i-1].Mode == 0))
      {
          HtmlCode += '<td>--</td>';
      }
      else
      {
          HtmlCode += '<td style="word-break:break-all;word-wrap:break-word;">' + LanArray[i-1].Vlan + '</td>'; 
      }
      
      HtmlCode += '</tr>'            
  }  
  
  return HtmlCode;    
}

function OnChooseDeviceType(Select)
{
   var Mode = getElementById("ChooseDeviceType").value;

   if (Mode == "lanwanbinding")
       getElById("BindVlanRow").style.display = 'none';        
   else if (Mode == "vlanbinding")
       getElById("BindVlanRow").style.display = '';
}

//用户点击了取消按钮后的代码逻辑
function OnCancelButtonClick()
{
    document.getElementById("TableUrlInfo").style.display = "none";
    return false;
}

function getEntryIndex(portid)
{
	switch ( portid )
	{
		case 'LAN1':
		case 'LAN2':
		case 'LAN3':
		case 'LAN4':
			return parseInt(portid.substring(3, 4)) - 1;
		case 'SSID1':
		case 'SSID2':
		case 'SSID3':
		case 'SSID4':
			return parseInt(portid.substring(4, 5)) + 3;
		default:
			return -1;
	}
}

function OnApplyButtonClick()
{
		var entryidx = 0;
		var PortId = getElementById("PortId").innerHTML;

		if (FillBindInfo() == false)
		  return false;

		entryidx = getEntryIndex(PortId);
		if ( entryidx < 0 )
		{
			alert('Invalid entry!')
			return false;	
		}
		document.forms[0].entryidx.value = entryidx;
		document.forms[0].formcommit.value = 1;
		document.forms[0].bindmode.value = ('vlanbinding' == getElById("ChooseDeviceType").value) ? 'Yes' : 'No';
		if ( 'Yes' == document.forms[0].bindmode.value )
			document.forms[0].bindvalue.value = getElById("UrlAddressControl").value;
		document.forms[0].submit();
		return true;
}

function FillBindInfo(Form)
{
	if (CheckParameter() == false)
		return false;

	return true;
}

function CheckParameter()
{
	var BindVlan = getElById("UrlAddressControl").value;
	
	var Mode = getElementById("ChooseDeviceType").value;
	if ((0 == BindVlan.length) && (Mode == "vlanbinding"))
	{
	    alert('必须设置VLAN对');
			return false;
	}

	if (Mode == "vlanbinding")
	{
		if (IsBindBindVlanValid(BindVlan) == false)
		{
			return false;
		}
	}

	return true;
}

function IsBindBindVlanValid(BindVlan)
{   
	var LanVlanWanVlanList = BindVlan.split(";");
	var LanVlan0;
	var WanVlan;
	var TempList;

	for (var i = 0; i < LanVlanWanVlanList.length; i++)
	{
		TempList = LanVlanWanVlanList[i].split("/");
		
		/* 检验是否满足a/b格式 */
		if (TempList.length != 2)
		{
			alert(BindVlan+"格式不合法。");
			return false;
		}
		
		/* 检验a，b是否为数字 */
		if ((isNaN(parseInt(TempList[0]))) || (isNaN(parseInt(TempList[1]))))
		{
			alert(BindVlan+"格式不合法。");
			return false;
		}
		
		/* 检验lan口的vlan是否合法 */
		if (!(parseInt(TempList[0]) >= 1 && parseInt(TempList[0]) <= 4094))
		{
			alert(BindVlan+"中用户Vlan\""+parseInt(TempList[0])+"\"不合法。");
			return false;
		}
		
		if (!(parseInt(TempList[1]) >= 1 && parseInt(TempList[1]) <= 4094))
		{
			alert(BindVlan+"中出接口Vlan\""+parseInt(TempList[1])+"\"不合法。");
			return false;
		}
	}

	if ( LanVlanWanVlanList.length > 20 )
	{
			alert("最大支持20个VLAN对。");
			return false;
	}

	return true;
}
</SCRIPT>      
 
<META content="MSHTML 6.00.6000.16809" name=GENERATOR></HEAD>
<BODY style="TEXT-ALIGN: center" vLink=#000000 aLink=#000000 link=#000000 
leftMargin=0 topMargin=0 
onload="DisplayLocation(getElement('Selected_Menu').value);FinishLoad();if(getElById('ConfigForm') != null)LoadFrame()" 
onunload=DoUnload() marginheight="0" marginwidth="0">
<TABLE height="100%" cellSpacing=0 cellPadding=0 width=808 align=center 
border=0>
  <TBODY>
  <TR>
    <TD height=1>
      <TABLE height=117 cellSpacing=0 cellPadding=0 width=808 
      background=/img/framelogo.jpg border=0>
        <TBODY>
        <TR>
          <TD>&nbsp;</TD>
          <TD vAlign=bottom align=right width=358>
            <TABLE id=table8 cellSpacing=0 cellPadding=0 border=0>
              <TBODY>
              <TR>
                <TD vAlign=bottom align=right><SPAN class=curUserName>&nbsp; 
                  </SPAN></TD>
                <TD class=welcom vAlign=bottom align=middle width=120>欢迎您！ </TD>
                <TD vAlign=bottom width=50><A onclick=DoLogout() 
                  href="/cgi-bin/logout.cgi" target=_top><SPAN 
                  class=logout>退出 
      </SPAN></A></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
      <TABLE id=table2 height=100 cellSpacing=0 cellPadding=0 width=808 
border=0>
        <TBODY>
        <TR>
          <TD class=LocationDisplay id=LocationDisplay align=middle width=163 
          bgColor=#ef8218 rowSpan=3></TD>
          <TD width=434 bgColor=#427594 height=33 class=CucBgColorTD_1>
            <P align=right><FONT face=黑体 color=#ffffff><B><FONT face=黑体 
            color=#ffffff size=6><INPUT id=Selected_Menu type=hidden 
            value="网络->绑定设置" name=Selected_Menu> </FONT></B><SPAN 
            class=GatewayName>网关名称:
            <SCRIPT 
            language=javascript>
document.write(top.gateWayName);
</SCRIPT>
             </SPAN></FONT></P></TD>
          <TD width=211 bgColor=#ef8218 height=33 class=CucBgColorTD_1>
            <P class=GatewayType align=center>型号:
            <SCRIPT language=javascript>
document.write(top.ModelName);
</SCRIPT>
             </P></TD></TR>
        <TR>
          <TD id=MenuArea_L1 vAlign=bottom bgColor=#ef8218 colSpan=2 
          height=43 class=CucBgColorTD_1>&nbsp;</TD></TR>
        <TR>
          <TD id=MenuArea_L2 bgColor=#427594 colSpan=2 
      height=24 class=CucBgColorTD_3></TD></TR></TBODY></TABLE>
      <SCRIPT 
      language=javascript>
MakeMenu(getElById ('Selected_Menu').value);
</SCRIPT>

      <TABLE id=table3 height=15 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD height=15><IMG height=15 src="/img/panel1.gif" 
            width=164 border=0> </TD>
          <TD><IMG height=15 src="/img/panel2.gif" width=645 
            border=0> </TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD vAlign=top>
      <TABLE height="100%" cellSpacing=0 cellPadding=0 border=0>
        <TBODY>
        <TR>
          <TD class=Item_L1 width=157 bgColor=#ef8218 height=30>
            <P>绑定模式</P></TD>
          <TD width=7 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=474>　</TD>
          <TD vAlign=top width=170 background=/img/panel4.gif 
          rowSpan=4>
            <TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
              <TBODY>
              <TR>
                <TD><A href="/cgi-bin/help_content.asp#绑定设置" 
                  target=_blank><IMG height=34 src="/img/help.gif" 
                  width=40 border=0></A></TD></TR></TBODY></TABLE></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7 height=10></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR>
        <TR>
          <TD class=Item_L2 vAlign=top width=157 bgColor=#e7e7e7 height=30>
            <P></P></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD>
          	<FORM name="ConfigForm" action="/cgi-bin/net-binding.asp" method="post">
						<!--解释头-->
						<div id="PromptPanel">
						    <table width="100%" border="0" cellpadding="0" cellspacing="0">
						        <tr>
						            <td class="prompt">
						                <table width="100%" border="0" cellspacing="0" cellpadding="0">
						                    <tr>
						                        <td width="100%" class="title_01" style="padding-left: 10px;">
						                            您可以进行VLAN绑定的操作，VLAN的值以m1/n1 VLAN对方式设置, 其中m1代表用户侧的vlan,n1代表出接口的vlan, 多组VLAN对以分号分开。
						                        </td>
						                    </tr>
						                </table>
						            </td>
						        </tr>
						        <tr>
						        <td height="5px"></td>
						        </tr>
						
						    </table>
						  
					</div>

					<!--TODO:在这里加入你的具体页面展现-->
					  <table class="table1_bg" border="0" cellspacing="1" style="margin-left: 5px;" width="98%">
					    <tr class="table1_title">
						       <td width="10%">端口</td>
						       <td width="15%">绑定模式</td>
						       <td width="75%">绑定vlan对</td>
						  </tr>    
						    <script>
						   document.write(CreateRouteList());              
						   </script>  
					  
					 </table>

					 <!--Apply和Cancel按钮-->
					  <div id="TableUrlInfo" style="display:none">
					
					  <table cellspacing="1" cellpadding="0" class="table1_bg" style="margin-left: 5px;" width="98%">
					  
					    <tr class="trTabConfigure">  
					    <td align= left width=25% class="table1_left">端口: <td  class="table1_right"> <div id="PortId"></div></td> </td>
					    </tr>
					</table>
					
					  <table cellspacing="1" cellpadding="0" class="table1_bg" style="margin-left: 5px;" width="98%">
					    <tr class="trTabConfigure">
					    <td  align=left width="25%" class="table1_left">端口模式:</td><td  class="table1_right"><select id="ChooseDeviceType" onchange="OnChooseDeviceType(this);"><option value="vlanbinding">VLAN绑定</option><option value="lanwanbinding">端口绑定</option></select>
					    </td>
					    </tr>
					  </table>
					  <div id="BindVlanRow">
					  <table cellspacing="1" cellpadding="0" class="table1_bg" style="margin-left: 5px;" width="98%">
					    <tr class="trTabConfigure"><td width="25%"  align=left class="table1_left">绑定VLAN对:</td><td class="table1_right"><input type=text id="UrlAddressControl"  style="width:300px" maxlength=255 /></td></tr>
					  </table>
					  </div>
					  <table id="ConfigPanelButtons" width="100%" cellspacing="1" class="table1_button">
					    <tr>
					        <td width="25%">
					        </td>
					        <td class="table1_submit" style="padding-left: 5px">
					        		<input type='hidden' value='0' name='entryidx'>
					        		<input type='hidden' value='0' name='formcommit'>
					        		<input type='hidden' value='0' name='bindmode'>
					        		<input type='hidden' value='0' name='bindvalue'>
					        		<input type='hidden' value='' name='empty'>
					            <input id="ButtonApply"  type="button" value="应用" onclick="javascript:return OnApplyButtonClick();" class="submit" />
					            <input id="ButtonCancel" type="button" value="取消" onclick="javascript:OnCancelButtonClick();" class="submit" />
					        </td>
					    </tr>
					  </table>
					</div>

            </FORM></TD></TR>
        <TR>
          <TD vAlign=top width=157 bgColor=#e7e7e7></TD>
          <TD width=7 background=/img/panel3.gif>　</TD>
          <TD></TD></TR></TBODY></TABLE></TD></TR>
  <TR>
    <TD height=1>
      <TABLE id=table7 height=35 cellSpacing=0 cellPadding=0 width=808 
        border=0><TBODY>
        <TR>
          <TD width=162 bgColor=#ef8218 class=CucBgColorTD_2>　</TD>
          <TD width=278 bgColor=#427594 class=CucBgColorTD_2>　</TD>
          <TD width=196 bgColor=#427594 class=CucBgColorTD_2>
            <P align=center></P></TD>
          <TD width=170 
bgColor=#313031 class=CucBgColorTD_2>　</TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE></BODY></HTML>
