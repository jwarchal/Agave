<h2>Edit Schemata</h2>
<p>Select which schema you would like to modify from the list of available schemata below.</p>

<ul>
<?php foreach($stems as $stem): ?>
	<li><a class='admin-schema-stem' href='<?php echo $agave->url."admin/schemata&stem=".$stem; ?>'><?php echo $stem ?></a></li>
<?php endforeach; ?>
</ul>
