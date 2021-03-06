<?php

/**
 * @file
 * Manages webform external administration UI
 */

/**
 * Callback function to edit default options
 */
function webform_salsa_config($form, &$form_state) {

	$form = array();

	$form['webform_salsa_node_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa node URL'),
		'#default_value' => variable_get('webform_salsa_node_url'),
	);
	$form['webform_salsa_organization_key'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa organization key'),
		'#default_value' => variable_get('webform_salsa_organization_key'),
	);
	$form['webform_salsa_chapter_key'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa chapter key'),
		'#default_value' => variable_get('webform_salsa_chapter_key'),
	);
	$form['webform_salsa_account_email'] = array(
		'#type' => 'textfield',
		'#title' => t('Email of Salsa user account'),
		'#default_value' => variable_get('webform_salsa_account_email'),
	);
	$form['webform_salsa_password'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa account password'),
		'#default_value' => variable_get('webform_salsa_password'),
	);
	$form['webform_salsa_group_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa group id'),
		'#default_value' => variable_get('webform_salsa_group_id'),
	);
	$form['webform_salsa_autoresponder_ids'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa autoresponder ids'),
		'#default_value' => variable_get('webform_salsa_autoresponder_ids'),
	);
	$form['webform_salsa_tag'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa tag'),
		'#default_value' => variable_get('webform_salsa_tag'),
	);

	return system_settings_form($form);
}

function webform_salsa_config_validate($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['webform_salsa_node_url'] !== '' && !valid_url($values['webform_salsa_node_url'],true)) {
    form_set_error('webform_salsa_node_url', t('Salsa node URL must be a valid URL.'));
  }
  $kid = $values['webform_salsa_organization_key'];
  if ($kid !== '' && (!is_numeric($kid) || intval($kid) != $kid)) {
    form_set_error('webform_salsa_organization_key', t('Salsa Organization Key must be an integer.'));
  }
  $kid = $values['webform_salsa_chapter_key'];
  if ($kid !== '' && (!is_numeric($kid) || intval($kid) != $kid)) {
    form_set_error('webform_salsa_chapter_key', t('Salsa Chapter Key must be an integer.'));
  }
  if ($values['webform_salsa_account_email'] !== '' && !valid_email_address($values['webform_salsa_account_email'])) {
    form_set_error('webform_salsa_account_email', t('Salsa Account Email must be a valid email.'));
  }
  $gid = $values['webform_salsa_group_id'];
  if ($gid !== '' && (!is_numeric($gid) || intval($gid) != $gid)) {
    form_set_error('webform_salsa_group_id', t('Salsa Group Id must be an integer.'));
  }
}


/**
 * Callback function to edit options on particular node
 */
function webform_salsa_manage_options($form, &$form_state, $node) {

  $form = array();
  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['oid'] = array(
    '#type' => 'hidden',
  );
  $form['salsa_enabled'] = array (
    '#type' => 'checkbox',
    '#title' => 'Enable Salsa integration',
  );
  $form['salsa_node_url'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa node URL (required)',
	'#default_value' => variable_get('webform_salsa_node_url'),
  );
  $form['salsa_organization_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa organization key (required)',
	'#default_value' => variable_get('webform_salsa_organization_key'),
  );
  $form['salsa_chapter_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa chapter key (optional)',
	'#default_value' => variable_get('webform_salsa_chapter_key'),
  );
  $form['salsa_account_email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email of Salsa user account (required)',
	'#default_value' => variable_get('webform_salsa_account_email'),
  );
  $form['salsa_password'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa account password (required)',
	'#default_value' => variable_get('webform_salsa_password'),
  );
  $form['salsa_field_mapping'] = array(
    '#type' => 'textarea',
    '#title' => 'Salsa Field Mapping (required)',
    '#description' => 'One field per line. Use format Salsa_Field|form_key.  If salsa field matches the form_key, you can just enter the key once, without the | character.',
  );
  $form['salsa_boolean_field_mapping'] = array(
    '#type' => 'textarea',
    '#title' => 'Salsa Checkbox to Boolean Field Mapping',
    '#description' => 'One form_key per line. If the value of the key from multiple checkboxes matches a Salsa Field key, that Salsa Field will be set to 1.',
  );
  $form['salsa_groups_field_mapping'] = array(
    '#type' => 'textarea',
    '#title' => 'Salsa Checkbox to Group Mapping',
    '#description' => 'One form_key per line. If the value of the key from multiple checkboxes matches a Salsa Group key, the contact will be added to that group.',
  );
  $form['salsa_group_id'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa Group Id (optional)',
    '#description' => 'If the id (key) of a Salsa group is entered here, the contact will be added to that group. Must be an integer.',
	'#default_value' => variable_get('webform_salsa_group_id'),
  );
  $form['salsa_autoresponder_ids'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa Autoresponder Ids (optional)',
    '#description' => 'If one or more ids (key) of a Salsa autoresponder is entered here (comma separated), the contact will be sent that autoresponder.',
	'#default_value' => variable_get('webform_salsa_autoresponder_ids'),
  );
  $form['salsa_tag'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa Tag (optional)',
    '#description' => 'If a tag is entered here, the contact will be tagged in Salsa.',
	'#default_value' => variable_get('webform_salsa_tag'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Options',
  );

  // get current settings
  $query = db_select('webform_salsa_options','o');
  $query->fields('o')->condition('nid',$node->nid);
  $result = $query->execute();
  foreach ($result as $field) {
      $form['oid']['#value'] = $field->oid;
      $form['salsa_enabled']['#default_value'] = $field->salsa_enabled;
      $form['salsa_node_url']['#default_value'] = $field->salsa_node_url ? $field->salsa_node_url
			: $form['salsa_node_url']['#default_value'];
      $form['salsa_organization_key']['#default_value'] = $field->salsa_organization_key
			? $field->salsa_organization_key : $form['salsa_organization_key']['#default_value'];
      $form['salsa_chapter_key']['#default_value'] = $field->salsa_chapter_key ? $field->salsa_chapter_key
			: $form['salsa_chapter_key']['#default_value'];
      $form['salsa_account_email']['#default_value'] = $field->salsa_account_email ? $field->salsa_account_email
			: $form['salsa_account_email']['#default_value'];
      $form['salsa_password']['#default_value'] = $field->salsa_password ? $field->salsa_password
			: $form['salsa_password']['#default_value'];
      $form['salsa_field_mapping']['#default_value'] = $field->salsa_field_mapping;
      $form['salsa_boolean_field_mapping']['#default_value'] = $field->salsa_boolean_field_mapping;
      $form['salsa_groups_field_mapping']['#default_value'] = $field->salsa_groups_field_mapping;
      $form['salsa_group_id']['#default_value'] = $field->salsa_group_id ? $field->salsa_group_id
			: $form['salsa_group_id']['#default_value'];
      $form['salsa_autoresponder_ids']['#default_value'] = $field->salsa_autoresponder_ids
			? $field->salsa_autoresponder_ids : $form['salsa_autoresponder_ids']['#default_value'];
      $form['salsa_tag']['#default_value'] = $field->salsa_tag ? $field->salsa_tag
			: $form['salsa_tag']['#default_value'];
  }

  return $form;

}

function webform_salsa_manage_options_validate($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['salsa_node_url'] !== '' && !valid_url($values['salsa_node_url'],true)) {
    form_set_error('salsa_node_url', t('Salsa node URL must be a valid URL.'));
  }
  $kid = $values['salsa_organization_key'];
  if ($kid !== '' && (!is_numeric($kid) || intval($kid) != $kid)) {
    form_set_error('salsa_organization_key', t('Salsa Organization Key must be an integer.'));
  }
  $kid = $values['salsa_chapter_key'];
  if ($kid !== '' && (!is_numeric($kid) || intval($kid) != $kid)) {
    form_set_error('salsa_chapter_key', t('Salsa Chapter Key must be an integer.'));
  }
  if ($values['salsa_account_email'] !== '' && !valid_email_address($values['salsa_account_email'])) {
    form_set_error('salsa_account_email', t('Salsa Account Email must be a valid email.'));
  }
  $gid = $values['salsa_group_id'];
  if ($gid !== '' && (!is_numeric($gid) || intval($gid) != $gid)) {
    form_set_error('salsa_group_id', t('Salsa Group Id must be an integer.'));
  }
}

function webform_salsa_manage_options_submit($form, &$form_state) {
  $values_to_set = array(
	'nid','salsa_enabled','salsa_node_url','salsa_organization_key','salsa_chapter_key','salsa_account_email',
	'salsa_password','salsa_field_mapping','salsa_boolean_field_mapping','salsa_groups_field_mapping','salsa_group_id',
	'salsa_autoresponder_ids','salsa_tag'
  );
  $values = $form_state['values'];
  // echo '</pre>';
  $record = new stdClass();
  foreach($values_to_set as $k) {
    if (isset($values[$k])) { $record->$k = $values[$k]; }
  }
  /* mail('jk@webskillet.com','$values and $record',print_r($values,1)."\n\n".print_r($record,1),"From: Webskillet Dev <dev@webskillet.com>\r\n"); */
  if ($values['oid']) {
    $record->oid = $values['oid'];
    drupal_write_record('webform_salsa_options', $record, 'oid');
  } else {
    drupal_write_record('webform_salsa_options', $record);
  }
  drupal_set_message(t('Configuration options have been saved'));
}
