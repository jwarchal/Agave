//TODO: document this thing, seriously, we should do this for all modules

fieldManager uses of the following types of extensions:
 - form
 - alterer
 - field
 - validator
 
 Form extension example:
 $extensions[] = array(
 	'module'=>'fieldManager',
 	'type'=>'form',
 	'location'=>'relative/location/of/your/file',
 	'data'=>array(
 		''=>''
 	),
 );