<?php
	$mediaPlayer = $agave->load('mediaPlayer');
?>

<div id='mo-video-player'>
	<?php //print $mediaPlayer->dispay('jw', $media->returnJWsettings($format)); ?>
	<h4>Media display to go here</h4>
</div>

<?php if($downloadOriginal || $downloadCompressed): ?>
<div id='mo-files'>
	<p>Use the links below to download the original, or web-compressed versions of this media.</p>
	<ul>
	<?php if($downloadOriginal): ?>
		<li><a href='<?php print $agave->url."media/download&mediaKey=$media->mediaKey&version=original" ?>'>original</a></li>	
	<?php endif; ?>
	<?php if($downloadCompressed): ?>
		<?php foreach($media->transcodedFiles as $preset=>$file): ?>
			<li><a href='<?php print $file->url ?>'><?php print $preset ?></a></li>
		<?php endforeach; ?>
	<?php endif; ?>
	</ul>
</div>
<?php endif; ?>

<?php if(!empty($media->transcript)): ?>
<div id='mo-transcript'>
	<?php print $media->transcript; ?>
</div>
<?php endif; ?>