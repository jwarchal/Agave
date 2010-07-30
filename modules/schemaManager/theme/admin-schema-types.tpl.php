<h2>Edit Schemata</h2>


<?php if($schemata): ?>
	<p>Select which schema you would like to edit from the list below.</p>
	<ul>
	<?php foreach($schemata as $schema): ?>
		<li>
			<a href='<?php echo $agave->url."admin/schemata&stem=$stem&schema=".$schema['schema']?>'><?php echo $schema['label'] ?></a>
			 - <?php print $schema['desc'] ?>
		</li>
	<?php endforeach; ?>
	</ul>
<?php else: ?>
	<p>You have no schemata to manage in this set of schema stems.</p>
<?php endif; ?>