<?php
	$this->addCSS('contentNode',"theme/css/node.css");
?>

<div class='node-wrapper node-wrapper-<?php echo $node->type ?>'>
	<?php if($editButton): ?>
		<a class='node-edit-button' href='<?php echo $agave->url."content/$node->contentNodeKey/edit" ?>'></a>
	<?php endif; ?>

	<?php foreach($node->fields->fieldOrderedList as $field): ?>
		<div class='node-field node-field-<?php echo $field ?>'>
			<?php print $node->fields->displayField($field); ?>
		</div>
	<?php endforeach; ?>
</div>