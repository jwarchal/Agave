<?php
function schemaManager_return_field_element_form($form_values = null, $data=FALSE) {
	global $agave;

	//parse data for field to edit
	$stem = $data['stem'];
	$fieldKey = $data['fieldKey'];
	$schema = $data['schema'];
	if($fieldKey == 0) $fieldKey = FALSE;

	//get values on requested field from db
	if($data) {
		if($fieldKey != '0') {
			$query = "SELECT * FROM `".$stem."_schema_fields` WHERE `fieldKey`='$fieldKey'";
			$field = $agave->doSQLQuery($query, 2);
		}
	}

	//start assembling form
	$form = array();
	$form['action'] = $agave->url.'admin/schemata/save/field';
	$form['method'] = 'post';
	$form['cancel'] = TRUE;

	//figure out which fields to add
	$elements = array();
	$elements['fieldName'] = array(
		'#type'=>'text',
		'#label'=>'Name of field',
		'#size'=>20,
		'#weight'=>0,
		'#default'=> ($fieldKey) ? $field['keyName'] : NULL,
		'#disabled'=> ($fieldKey) ? TRUE : FALSE,
	);
	$elements['type'] = array(
		'#type'=>'select',
		'#label'=>'Form element',
		'#values'=> $agave->load('fieldManager')->getAvailableFields(),
		'#default'=>($fieldKey) ? $field['type'] : 'text',
		'#weight'=>1,
		'#disabled'=> ($fieldKey) ? TRUE : FALSE,
	);
	$elements['label'] = array(
		'#type'=>'text',
		'#label'=>"Label",
		'#size'=>20,
		'#weight'=>2,
		'#default'=> ($fieldKey) ? $field['label'] : NULL,
	);
	$label = (isset($field['showLabel'])) ? array('show') : NULL; //figure true/false for showlabel
	$elements['showLabel'] = array(
		'#type'=>'checkbox',
		'#label'=>'Show label',
		'#values'=>array('show'),
		'#nolabel'=>true,
		'#weight'=>3,
		'#default'=> ($fieldKey) ? $label : array('show'),
	);
	$elements['helpText'] = array(
		'#type'=>'text',
		'#size'=>20,
		'#label'=>'Help text',
		'#default'=> ($fieldKey) ? $field['help'] : NULL,
	);
	$elements['access'] = array(
		'#type'=>'text',
		'#label'=>"Access argument",
		'#size'=>20,
		'#weight'=>4,
		'#default'=> ($fieldKey) ? $field['access'] : NULL,
	);
	$elements['schema'] = array(
		'#type'=>'select',
		'#label'=>"Schema",
		'#help'=>'Select which schemata this field applies to.',
		'#size'=>20,
		'#weight'=>4,
		'#values'=>($data) ? $agave->load('schemaManager')->availableSchemata($stem) : NULL,
		'#default'=> ($fieldKey) ? $field['schema'] : $schema,
	);
	//add hidden elements for stem/fieldKey
	$elements['stem'] = array(
		'#type'=>'hidden',
		'#value'=>$stem,
	);

	$elements['fieldKey'] = array(
		'#type'=>'hidden',
		'#value'=>$fieldKey,
	);

	$form['fields'] = $elements;
	return $form;
}

function schemaManager_return_field_element_settings_form($form_values = null, $data=FALSE) {
	global $agave;

	//parse data for requested field
	$stem = $data['stem'];
	$fieldKey = $data['fieldKey'];
	$type = $data['type'];
	if($fieldKey == 0) $fieldKey = FALSE;
	if(!$fieldKey) return FALSE;

	//figure out what type of field this is to get settings form from fieldManager
	$query = "SELECT `type`,`settings` FROM `".$stem."_schema` WHERE `fieldKey`='$fieldKey'";
	$data = $agave->doSQLQuery($query, 2);

	if(!$data) return FALSE;
	$field = $data['type'];
	$settings = $data['settings'];

	//define the form data
	$form = array();
	$form['action'] = $agave->url.'admin/schemata/save/field/settings';
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