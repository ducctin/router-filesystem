<html><head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<link rel="stylesheet" type="text/css" href="/style.css" tppabs="http://192.168.1.1/css/style.css">
</head><body>
<div align=center>
<table width="760" height="5" border="0" align=center cellpadding="0" cellspacing="0" class="orange">
  <tr>
    <td></td></tr></table><table border="0" width="760" cellspacing="0" cellpadding="0" bgcolor="#FFFFFF">
  <tr>
    <td width="200" height="50" align=center valign=middle bgcolor="#FFFFFF"><div align=left>
	<img src="/logo.gif" tppabs="logo.gif" width="200" height="50">
	</div></td><td width="560" align=right valign="bottom" bgcolor="#FFFFFF" class="model">
	  <font color="#000000">
	  <%if tcWebApi_get("WebCustom_Entry","isProlineCmdAction","h") = "Yes" then %>
	  <%if tcWebApi_get("SysInfo_Entry","ProductName","h") <> "N/A" then tcWebApi_get("SysInfo_Entry","ProductName","s") else%>
	  	<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") = "Yes" then%>xPON Router<%else%>
	  <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>VDSL/<%end if%>ADSL Router
	  <%end if%>	  
		<%end if%>	  
	  <%else%>
		<%if tcWebApi_get("WebCustom_Entry", "haveXPON", "h") = "Yes" then%>xPON Router<%else%>
	  <%if tcWebApi_get("WebCustom_Entry","havePtm","h") = "Yes" then%>VDSL/<%end if%>ADSL Router	  
	  	<%end if%> 	  
	  <%end if%> 	  
	  </font>	
	  </td></tr></table><table width="760" height="1" border="0" align=center cellpadding="0" cellspacing="0" bgcolor="#CCCCCC">
  <tr>
    <td></td></tr></table></div>
</body></html>
