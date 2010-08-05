<?php
	//default form styles, and schemaManager styles
	$this->addCSS('fieldManager','theme/css/fm-defaults.css');
	$this->addCSS('admin','theme/css/schema-manager.css');
	
	//add jQuery UI dependencies
	$this->addJS(NULL,NULL,NULL,'vendor/jQuery/ui/minified/jquery-ui.min.js');
	$this->addJS(NULL,NULL,NULL,'vendor/jQuery/ui/minified/ui.draggable.min.js');
	$this->addJS(NULL,NULL,NULL,'vendor/jQuery/ui/minified/ui.sortable.min.js');

	//schemaManager js
	$this->addJS('schemaManager','js/schema-manager.js');
?>
<h2>Editing <em><b><?php print $stem ?></b></em> Schema "<?php print $schema ?>"</h2>
<p>You can add/delete/modify fields for the chosen schema below.  Drag and drop the fields to reorder their position on the page/edit form.</p>

<small>
	<ul>
		<strong>Notes:</strong>
		<li>You cannot rename fields once you create them, there could be unintended consequences in the database.</li>
		<li>Modifying shared fields (marked in red) will affect all schemata in which that field appears.</li>
		<li>Some fields may be locked (striped border) because they have been coded against in other modules.  To make changes (DANGEROUS!), you must use phpmyadmin to either unlock them, or change them in the database.</li>
	</ul>
</small>

<div id='schema-field-container'>
	<a id='new-field'>
		Add field
		<input class='fieldmeta' type='hidden' value='<?php echo $stem.".0.".$schema ?>' />
	</a>
	<ul id='schema-field-list'>
		<?php if($fields): foreach($fields as $field): ?>
			<?php $listItemClass = ($field['schema'] != $schema) ? "class='schema-field schema-shared-field'" : "class='schema-field'"; ?>
			<li <?php echo $listItemClass ?> id='<?php echo $stem."_".$field['fieldKey'] ?>'>
				<span class='schema-field-keyName'><?php echo $field['keyName'] ?><?php if($field['locked']): ?> (locked) <?php endif; ?></span>
				<?php if(!$field['locked']): ?><a class='schema-field-delete schema-field-control'>delete</a><?php endif; ?>
				<?php if(!$field['locked']): ?><a class='schema-field-edit schema-field-control'>edit</a><?php endif; ?>
				<?php if(!$field['locked']): ?><a class='schema-field-control show-field-settings'>settings</a><?php endif; ?>
				<input class='schema-field-metadata' type='hidden' value='<?php echo $stem.".".$field['fieldKey'].".".$field['schema'] ?>' />
			</li>
		<?php endforeach; else: print "No fields :("; endif; ?>
	</ul>
</div>