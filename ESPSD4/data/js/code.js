// ScouseElectron ESP8266 update JS

window.onload = function(){
	window.onclick = function(event){
		if (event.target == mbox){
			mbox.style.display = "none";
		}
	  } 
	var surl = './settings';
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function(){
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200){
            try{
                var data = JSON.parse(xmlhttp.responseText);
            } catch(err){
                console.log(err.message + " in " + xmlhttp.responseText);
                return;
            }
            UpdateData(data);
		}
	};
    xmlhttp.open("GET", surl, true);
    xmlhttp.send();
};

function UpdateData(data){
	if("wifissid" in data){ document.getElementById("wifissid").value = data["wifissid"]; }
	if("wifipass" in data){	document.getElementById("wifipass").value = data["wifipass"]; }
	if("ipaddr" in data){ document.getElementById("ipaddr").value = data["ipaddr"]; }
	if("netmask" in data){ document.getElementById("netmask").value = data["netmask"]; }
	if("gateway" in data){ document.getElementById("gateway").value = data["gateway"]; }
	if("dns1" in data){	document.getElementById("dns1").value = data["dns1"]; }
	if("dns2" in data){	document.getElementById("dns2").value = data["dns2"]; }
	if("ipmode" in data){
		if (data["ipmode"] == "0"){
			document.getElementById("static").checked = true;
			ipInputEnable();
		} else {
			document.getElementById("dhcp").checked = true;
			ipInputDisabled();
		}
	}
}

function ipInputEnable(){
	document.getElementById("ipaddr").disabled = false;
	document.getElementById("netmask").disabled = false;
	document.getElementById("gateway").disabled = false;
	document.getElementById("dns1").disabled = false;
	document.getElementById("dns2").disabled = false;
	document.getElementById("ipselector").style.display = "block";
}

function ipInputDisabled(){
	document.getElementById("ipaddr").disabled = true;
	document.getElementById("netmask").disabled = true;
	document.getElementById("gateway").disabled = true;
	document.getElementById("dns1").disabled = true;
	document.getElementById("dns2").disabled = true;
	document.getElementById("ipselector").style.display = "none";
}

function vFormCheck(){
	if (!SSIDisValid(document.getElementById("wifissid").value)){
		mboxShow("WiFi SSID apears invalid");
		return false;
	}
	if (!WPAisValid(document.getElementById("wifipass").value)){
		mboxShow("Passphrase apears invalid");
		return false;
	}
	if (document.getElementById("dhcp").checked == true){
		return true;
	}
	if (!IP4Valid(document.getElementById("ipaddr").value)){
		mboxShow("IP Address is invalid");
		return false;
	}
	if (!IP4Valid(document.getElementById("netmask").value)){
		mboxShow("Netmask is invalid");
		return false;
	}
	if (!IP4Valid(document.getElementById("gateway").value)){
		mboxShow("Gateway is invalid");
		return false;
	}
	if (!IP4Valid(document.getElementById("dns1").value)){
		mboxShow("DNS server is invalid");
		return false;
	}
	const i = document.getElementById("dns2");
	if (i.value != ""){
		if (!IP4Valid(i.value)){
			mboxShow("2nd DNS server is invalid");
			return false;
		}
	}
	return true;
}


function SSIDisValid(s){
	// FIXME:
	if (s.length < 4 || s.length > 31){
		return false;
	}
	if (/^[!#;].|[+\[\]/"\t\s].*$/.test(s)){
		return false;
	}
	return true;
}

function WPAisValid(s){
	// FIXME:
	if (s.length < 4 || s.length > 63){
		return false;
	}
	return /^[\u0020-\u007e\u00a0-\u00ff]*$/.test(s);
}

function PassIsValid(s){
	// FIXME: Use same as WPA perhaps ?!
	return WPAisValid(s);
}

function IP4Valid(s){
	if (/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test(s)){
	  return true;
	}
	return false;
}

function mboxClose(){
	document.getElementById("mbox").style.display = "none";
}

function mboxShow(msg){
	document.getElementById("mboxmgs").innerText = msg;
	document.getElementById("mbox").style.display = "block";
}

function vPassCheck(){
	const pas1 = document.getElementById("pas1").value;
	const pas2 = document.getElementById("pas2").value;
	const pas3 = document.getElementById("pas3").value;
	if (!PassIsValid(pas1)){
		mboxShow("Current password is invalid");
		return false;
	}
	if (!PassIsValid(pas2)){
		mboxShow("New password is invalid");
		return false;
	}
	if (pas2 != pas3){
		mboxShow("Passwords don't match");
		return false;
	}
	return true;
}
