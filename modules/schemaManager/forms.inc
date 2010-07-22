<?php
function schemaManager_return_field_element_form($data=FALSE) {
	global $agave;

	//parse data for field to edit
	$table = $data['table'];
	$nodeKey = $data['nodeKey'];
	$type = $data['type'];
	if($nodeKey == 0) $nodeKey = FALSE;

	//get values on requested field from db
	if($nodeKey != '0') {
		$query = "SELECT * FROM `".$table."_schema` WHERE `nodeKey`='$nodeKey'";
		$field = $agave->doSQLQuery($query, 2);
	}

	//start assembling form
	$form = array();
	$form['action'] = $agave->base_url.'admin/schemata/save/field';
	$form['method'] = 'post';
	$form['cancel'] = TRUE;

	//figure out which fields to add
	$elements = array();
	$elements[] = array(
		'#name'=>'fieldName',
		'#type'=>'text',
		'#label'=>'Name of field',
		'#size'=>20,
		'#weight'=>0,
		'#default'=> ($nodeKey) ? $field['keyName'] : NULL,
		'#disabled'=> ($nodeKey) ? TRUE : FALSE,
	);
	$elements[] = array(
		'#name'=>'type',
		'#type'=>'select',
		'#label'=>'Form element',
		'#values'=> $agave->load('fieldManager')->fields,
		'#default'=>($nodeKey) ? $field['type'] : 'text',
		'#weight'=>1,
		'#disabled'=> ($nodeKey) ? TRUE : FALSE,
	);
	$elements[] = array(
		'#name'=>'label',
		'#type'=>'text',
		'#label'=>"Label",
		'#size'=>20,
		'#weight'=>2,
		'#default'=> ($nodeKey) ? $field['label'] : NULL,
	);
	$label = (isset($field['showLabel'])) ? array('show') : NULL; //figure true/false for showlabel
	$elements[] = array(
		'#name'=>'showLabel',
		'#type'=>'checkbox',
		'#label'=>'Show label',
		'#values'=>array('show'),
		'#nolabel'=>true,
		'#weight'=>3,
		'#default'=> ($nodeKey) ? $label : array('show'),
	);
	$elements[] = array(
		'#name'=>'helpText',
		'#type'=>'text',
		'#size'=>20,
		'#label'=>'Help text',
		'#default'=> ($nodeKey) ? $field['help'] : NULL,
	);
	$elements[] = array(
		'#name'=>'access',
		'#type'=>'text',
		'#label'=>"Access argument",
		'#size'=>20,
		'#weight'=>4,
		'#default'=> ($nodeKey) ? $field['access'] : NULL,
	);
	$elements[] = array(
		'#name'=>'types',
		'#type'=>'text',
		'#label'=>"Types",
		'#help'=>'Enter a comma delimited string for the types this field should affect. TODO: change to checkboxes',
		'#size'=>20,
		'#weight'=>4,
		'#default'=> ($nodeKey) ? $field[$table.'_type'] : $type,
	);
	//add hidden elements for table/nodeKey
	$elements[] = array(
		'#type'=>'hidden',
		'#name'=>'stem',
		'#value'=>$table,
	);
	if($nodeKey) {
		$elements[] = array(
			'#type'=>'hidden',
			'#name'=>'nodeKey',
			'#value'=>$nodeKey,
		);
	}

	$form['fields'] = $elements;
	return $form;
}

function schemaManager_return_field_element_settings_form($data=FALSE) {
	global $agave;

	//parse data for requested field
	$table = $data['table'];
	$nodeKey = $data['nodeKey'];
	$type = $data['type'];
	if($nodeKey == 0) $nodeKey = FALSE;
	if(!$nodeKey) return FALSE;

	//figure out what type of field this is to get settings form from fieldManager
	$query = "SELECT `type`,`settings` FROM `".$table."_schema` WHERE `nodeKey`='$nodeKey'";
	$data = $agave->doSQLQuery($query, 2);

	if(!$data) return FALSE;
	$field = $data['type'];
	$settings = $data['settings'];

	//define the form data
	$form = array();
	$form['action'] = $agave->base_url.'admin/schemata/save/field/settings';
	$form['method'] = 'post';
	$form['cancel'] = TRUE;

	$fields = $agave->load('fieldManager')->returnSettingsForm($field, $settings);
	$fields[] = array(
		'#name'=>'fieldmeta',
		'#type'=>'hidden',
		'#value'=>implode(".", $data),
	);

	$form['fields'] = $fields;
	return $form;
}