<?php
function contentNode_return_edit_form($form_values, $node=NULL) {
	global $agave;

	if(!$node) agave_error_page('404', "Trying to load the <em>contentNode_edit</em> form with NO DATA!  Inconceivable!");
	$newItem = (is_object($node)) ? FALSE : TRUE;
	$nodeKey = ($newItem) ? NULL : $node->contentNodeKey;
	$nodeType = ($newItem) ? $node : $node->type;

	//define basic form attributes
	$form = array();
	$form['method'] = 'post';
	$form['files'] = TRUE;
	$form['cancel'] = TRUE;

	//get form elements from schema
	if($newItem) {
		//if new, load blank schema
		$schema = new schema("contentNode", $nodeType);
	}
	$fields = ($newItem) ? $schema->returnFormElements() : $node->fields->returnFormElements();

	//append other elements
	$fields['contentNodeKey'] = array(
		'#type'=>'hidden',
		'#value'=>$nodeKey,
	);
	$fields['type'] = array(
		'#type'=>'hidden',
		'#value'=>$nodeType,
		'#metadata'=> array(
			'table'=>'contentNode',
			'field'=>'type'
		)
	);
	$fields['meta'] = array(
		'#type'=>'fieldset',
		'#label'=>'Metadata',
		'#weight'=>50,
		'#collapsible'=>TRUE,
		'#collapsed'=>TRUE
	);
	$fields['meta']['published'] = array(
		'#type'=>'radio',
		'#label'=>'Published?',
		'#values'=>array('yes'=>'1', 'no'=>'0'),
		'#default'=>(isset($node->published)) ? '1' : '0',
		'#metadata'=>array (
			'table'=>'contentNode',
			'field'=>'published'
		)
	);

	$form['fields'] = $fields;

	return $form;
}