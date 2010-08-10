function displayNewMsg(messageText,proctorName,time,isNew,chatSessionKey) {	
var tbl = document.getElementById('transcript_' + chatSessionKey);

// toggle bgcolor for each new msg
var bgColor = ( window['bgToggleChat_' + chatSessionKey] == 1 ) ? "#EEEEEE" : "#FFFFFF";

var newRow1 = tbl.insertRow(-1);	// insert row at the end of the table
var authorCell = newRow1.insertCell(0);
var msgCell = newRow1.insertCell(1);


var newRow2 = tbl.insertRow(-1);
var timeCell = newRow2.insertCell(0);

// // create TextNodes
var authorText = document.createTextNode(trimWhiteSpace(proctorName)+":");
var paragraphs=messageText.split("<BR>");
var paragraphDivs = new Array();
	for (var i=0;i<paragraphs.length;i++) {
	paragraphDivs[i] = document.createElement('div');
	paragraphDivs[i].appendChild(document.createTextNode(paragraphs[i]));
	}
// Digs up the nodeKey from the text body if there is any
// 88 is a HACK!!!! if the prefix text length has been changed, then 88 needs to change too.
var nodeKey = messageText.substr(88,7);
// two arrays of the nodeKeys and the correspond text
var nodeKeyList = new Array('4081526','2372322','1961003');
var postFixList = new Array('Troubleshooting Introduction','Download Folder Permissions','Download Local-File Read Access File');
for (var k=0;k<nodeKeyList.length;k++){
// isStandardHREFMsg = True will be used to create DOM objects for standard message Link and image
	if (nodeKey == nodeKeyList[k]) {
	var isStandardHREFMsg = true;
	// grab the the text body correspond to the nodeKey
	var prefixText = postFixList[k];
	}
}
var timeText = document.createTextNode(time);
// append TextNodes
authorCell.appendChild(authorText);
// Create the message base on whether it is a normal chat or it is a link to the Manual
	if (isStandardHREFMsg) {
	var prefixElement = document.createTextNode('For more information please proceed to the following link ');
	var postfixElement = document.createTextNode(prefixText);
	var linkHREF = document.createElement('a');
	linkHREF.href='javascript:openManual(' + nodeKey + ')';
	linkHREF.className = 'inLineLink';
	linkIcon = document.createElement('img');
	linkIcon.src = 'images/linkIcon.gif'
	linkIcon.width = 12;
	linkIcon.height = 12;
	linkIcon.style.border = 'none';
	linkHREF.appendChild(linkIcon);
	linkHREF.appendChild(postfixElement);
	msgCell.appendChild(prefixElement);
	msgCell.appendChild(linkHREF);
	}
	else{
		for (var i=0;i<paragraphDivs.length;i++) {
		msgCell.appendChild(paragraphDivs[i]);
		}
	}
timeCell.appendChild(timeText);
// set style and tbl attributes
msgCell.width = "95%";
msgCell.valign = "middle";
msgCell.rowSpan="2";
msgCell.style.fontFamily="Arial";
msgCell.style.fontSize ="10pt";
msgCell.style.paddingLeft="15px";
msgCell.style.paddingTop="5px";
msgCell.style.paddingBottom="5px";
msgCell.style.backgroundColor= bgColor;
	if (isNew == 1) {
	msgCell.style.fontWeight="bold";
	}

authorCell.width = "5%";
authorCell.height = "5px";
authorCell.valign = "top";
authorCell.style.paddingLeft="10px";
authorCell.style.paddingTop="5px";
authorCell.style.fontWeight="bold";
authorCell.style.fontFamily="Arial";
authorCell.style.fontSize="10pt";
authorCell.style.backgroundColor= bgColor;

timeCell.style.paddingLeft="10px";
timeCell.style.paddingBottom="5px";
timeCell.style.fontFamily="Arial"; 
timeCell.style.fontSize='x-small';  
timeCell.valign = "top";  
timeCell.style.backgroundColor= bgColor;
// reset bgToggle flag to alternate color
window['bgToggleChat_' + chatSessionKey] = (window['bgToggleChat_' + chatSessionKey] == 0) ? 1 : 0;
	
scrollToEOP(chatSessionKey);
}
function trimWhiteSpace(s) {
// Remove leading spaces and carriage returns
	while ((s.substring(0,1) == ' ') || (s.substring(0,1) == '\n') || (s.substring(0,1) == '\r')) {
	s = s.substring(1,s.length);
	}
// Remove trailing spaces and carriage returns
	while ((s.substring(s.length-1,s.length) == ' ') || (s.substring(s.length-1,s.length) == '\n') || (s.substring(s.length-1,s.length) == '\r')) {
	s = s.substring(0,s.length-1);
	}
return s;
}
// Forces the chat-transcript window to scroll to the end-of-page, keeping the most recently posted message in view
function scrollToEOP(chatSessionKey) {
var scroll = $("#chatTranscript_" + chatSessionKey).attr("scrollHeight");	
	
$("#chatTranscript_" + chatSessionKey).scrollTop(scroll);
}
