<?php
/*
 * Template takes $user and $data (from contentNode_types table)
 * 
 */
?>
<p>Select the type of content you wish to create below.</p>
<ul>
<?php foreach($data as $datum): ?>
	<?php if($user->access("create_content") && $user->access("create_content_".$datum['schema'])): ?>
	<li>
		<a href='<?php echo $agave->url."content/create/".$datum['schema'] ?>'>
			<?php echo $datum['label'] ?>
		</a> - <?php echo $datum['desc'] ?>
	</li>
	<?php endif; ?>
<?php endforeach; ?>
</ul>