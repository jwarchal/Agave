<?php
function contentNode_return_edit_form($node=NULL) {
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
	//$form['multistep'] = TRUE;  //TODO: Julian help! - Is there any reason we can't define multistep here? instead of passing it via getFrorm?

	//get form elements from schema
	if($newItem) {
		//if new, load blank schema
		$schema = new schema("contentNode", $nodeType);
	}
	$fields = ($newItem) ? $schema->returnFieldElements() : $node->fields->returnFieldElements();

	//append other elements
	$fields[] = array(
		'#type'=>'hidden',
		'#name'=>'contentNodeKey',
		'#value'=>$nodeKey,
	);
	$fields[] = array(
		'#type'=>'hidden',
		'#name'=>'type',
		'#value'=>$nodeType,
		'#metadata'=> array(
			'table'=>'contentNode',
			'field'=>'type'
		)
	);
	$fields[] = array(
		'#type'=>'fieldset',
		'#name'=>'meta',
		'#label'=>'Metadata',
		'#weight'=>50,
		'#collapsible'=>TRUE,
		'#collapsed'=>TRUE
	);

//TODO: Julian help! How do I control the structure?  I want published to be a child of meta...
	$fields[] = array(
		'#type'=>'radio',
		'#name'=>'published',
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