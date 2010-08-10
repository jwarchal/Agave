
<?php

	$this->addCSS('chat','theme/css/chat.css');
	
	$this->addJS('chat','theme/js/displayMessages.js');
	$this->addJS('chat','theme/js/hoverbox.js');

?>

<script type="text/javascript">

	// Print themevars into JS vars

	var userKey = <?php print $userKey; ?>;

	var userMap_<?php print $sessionKey; ?> = <?php print json_encode($userMap); ?>;

	var sessionMap_<?php print $sessionKey; ?> = <?php print json_encode($sessionMap); ?>;

	var bgToggleChat_<?php print $sessionKey; ?> = <?php print $bgToggleChat; ?>;

	var unreadMessages_<?php print $sessionKey; ?> = <?php print $unreadMessages; ?>;

	var gopherInterval_<?php print $sessionKey; ?> = <?php print $gopherInterval; ?>;
	
	var messageKey_<?php print $sessionKey; ?> = <?php print $messageKey; ?>;
	
	var lastUpdateTime_<?php print $sessionKey; ?> = <?php print $lastUpdateTime; ?>;
	
	$(document).ready(function() {
	    displayMessages(<?php print json_encode($chatMsgs); ?>);	// display previous messages

		var gopherTimer_<?php print $sessionKey; ?> = setTimeout("gopher(<?php print $sessionKey; ?>)",  <?php print $gopherInterval; ?>);
		
		$(".icon").hoverbox();	// enable hovering over the status icons
		
		$("input").removeAttr("disabled");	// once the page is done loading, allow users to post messages
	});
	
	/*	Belongs in "viewMultipleChats," outside the scope of a single chat. Manages showing and hiding.    
		May require restricting selectors further.
		
		function initialize() {							
		$('div.Chat:eq(0) > div:gt(0)').hide();  

			$('div.Chat:eq(0) > h3').click(function() {
				$(this).next('div:hidden').slideDown('fast')
					.siblings('div:visible').slideUp('fast');

				var selectedSession = $(this).next('div').attr("id");
			});
		}
	*/
	
</script>
	
	<div class="Chat">
		<h3>
			<table>
				<tr>
					<td><b>Session <?php print $sessionKey; ?></b></td>
					<td>&nbsp;&nbsp;&nbsp;</td>
					
					<?php for($i = 0; $i < count($sessionMap); $i++): ?>
					<td>
						<table>
							<tr>
								<td><?php $name = explode(" ", $userMap[$sessionMap[$i]]);	print $name[0]; ?></td>
							</tr>
							<tr>
								<td> 
									<div id="status_<?php print $sessionKey . '_' . $sessionMap[$i]; ?>" class="<?php print $statuses[$i]['status']; ?>"></div>
								</td>
								<td>									
									<div id="icon_<?php print $sessionKey . '_' . $sessionMap[$i]; ?>" class="icon"></div>
								</td>
							</tr>
						</table>
					</td>
				    <?php endfor; ?>
				</tr>
			</table>
		</h3>
		
		<div id="<?php print $sessionKey; ?>">
			<textarea id="message_<?php print $sessionKey;?>" rows="3" cols="50" onkeydown="javascript:enter(<?php print $sessionKey; ?>,event);"></textarea>
			
			<input type="button" value="Submit" onClick="javascript:uploadMsg(<?php print $sessionKey; ?>, 1);" disabled="disabled" />
			
			<h4>Chat Transcript</h4>
			
			<div class="chatTranscript" id="chatTranscript_<?php print $sessionKey; ?>">
				<table id="transcript_<?php print $sessionKey; ?>">
				</table>
			</div>
			<br />
		</div> 
	</div>
