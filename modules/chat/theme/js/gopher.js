function gopher(chatSessionKey) {	
uploadMsg(chatSessionKey, 0);	
}

function uploadMsg(chatSessionKey, isFinal) {
	if (isFinal) {
		if ($("#message_" + chatSessionKey).val() == "") {	// Don't allow uploading a blank message
		return;
		}

		else {
		clearTimeout(window['gopherTimer_' + chatSessionKey]);	// Pause the routine gopher loop to avoid duplication
		
		$.ajaxq("gopher" + chatSessionKey);	// Clear the AJAX queue for the same reason 
		}	
	}

	$.ajaxq("gopher" + chatSessionKey, {
		async: true,
		type: "POST",
		url: agave.base_url+"chat/uploadMessage",
		data: {
		message    		: $("#message_" + chatSessionKey).val(), 
		messageKey 		: window['messageKey_' + chatSessionKey],
		sessionList 	: JSON.stringify(window['sessionMap_' + chatSessionKey]),	
		chatSessionKey  : chatSessionKey,	
		isFinal    		: isFinal,
		userKey			: userKey	// authorKey
		},
		beforeSend: function() {
		    if (isFinal) {
			window['messageKey_' + chatSessionKey] = "";	// Clear the chatMessageKey since the current one is now "archived" as final
				
			$("#message_" + chatSessionKey).val("");	// If its final, clear the textfield to indicate its been sent 
		    }
		},
		success: function(data) {	
		    if (isFinal) {
			window['gopherTimer_' + chatSessionKey] = setTimeout("gopher(" + chatSessionKey + ")", window['gopherInterval_' + chatSessionKey]);		
		    }
			   
		    else {
		    window['messageKey_' + chatSessionKey] = data;	// If its a tempMsg, set the msgKey var to it so further tempMsgs don't create an abundance
		
		    processStatuses(chatSessionKey);
			}
		},
		dataType: "text"	
	});
}

function processStatuses(chatSessionKey) {
	$.ajaxq("gopher" + chatSessionKey, {
		async: true,
		type: "GET",
		url: agave.base_url+"chat/processStatuses",
		data: { 
		chatSessionKey : chatSessionKey	
		},
		success: function(data) {
		displayStatuses(data);
		
		getNewMsgs(chatSessionKey);
		},
		dataType: "json"
	});
}

function getNewMsgs(chatSessionKey) {
	$.ajaxq("gopher" + chatSessionKey, {
		async: true,
		type: "GET",
		url: agave.base_url+"chat/getNewMsgs",
		data: { 
		lastUpdateTime : window['lastUpdateTime_' + chatSessionKey],
		chatSessionKey : chatSessionKey,
		userKey		   : userKey	
		},
		success: function(data) {
		window['lastUpdateTime_' + chatSessionKey] = data.lastUpdateTime;	

		displayMessages(data.chatMsgs);
		
		window['gopherTimer_' + chatSessionKey] = setTimeout("gopher(" + chatSessionKey + ")", window['gopherInterval_' + chatSessionKey]);
		},
		dataType: "json"
	});
}

function displayStatuses(data) {
	for (x in data) {
	$("#status_" + data[x].session + "_" + data[x].author).removeClass().addClass(data[x].status);	// Adds class which sets the proper icon 
	}	
}

function displayMessages(data) {	
	for (x in data)	{
    	if (data[x].isFinal == "1") {
		displayNewMsg(data[x].message, window['userMap_' + data[x].session][data[x].author], data[x].time, data[x].isNew, data[x].session);
		
		var selectedSession = $('div.Chat > div:visible').attr("id");
		
			if (selectedSession != data[x].session) {
			window['unreadMessages_' + data[x].session] = true;	
			
				// If another session delivers a newMsg, set a newMsg icon over an isTyping icon
				if ($("#icon_" + data[x].session + "_" + data[x].author).hasClass('writing')){
				$("#icon_" + data[x].session + "_" + data[x].author).toggleClass('writing newMessage');
				}
				
				else {
				$("#icon_" + data[x].session + "_" + data[x].author).addClasss('newMessage');
				} 
			}	
		}
	
	    else {
		$("#icon_" + data[x].session + "_" + data[x].author).data("tempMsg",data[x].message);	// Store temp message for tooltip
		
			if (data[x].message.length > 0) {	// If not a blank message..
			
				//If theres another message being typed after a new message was posted, override it to display the isTyping icon
				if ($("#icon_" + data[x].session + "_" + data[x].author).hasClass('newMessage')) {
				$("#icon_" + data[x].session + "_" + data[x].author).toggleClass('newMessage writing');
				}
				
				else {
				$("#icon_" + data[x].session + "_" + data[x].author).addClass('writing');		
				}	
			}	
			
			else if (window['unreadMessages_' + data[x].session] == true) {	
				// Otherwise, turn of the writing icon and put back the newMsg icon if there is an unread message
				if ($("#icon_" + data[x].session + "_" + data[x].author).hasClass('writing')){
				$("#icon_" + data[x].session + "_" + data[x].author).toggleClass('writing newMessage');
				}
				
				else {
				$("#icon_" + data[x].session + "_" + data[x].author).addClasss('newMessage');
				}
			}
			
			else {	// If no new messages and none are being typed, clear the icons 
			$("#icon_" + data[x].session + "_" + data[x].author).removeClass().addClass('icon');
			} 
		}
	}
}

function enter(chatSessionKey, e) {	
	// allows you to hit enter instead of clicking submit
	if(e.keyCode == 13) {
	e.preventDefault();
	
	uploadMsg(chatSessionKey, 1);
	}
}