<?php

/**
 * @file
 * Administrative form for setting Simple Access Grants Profiles.
 */

/**
 * Access Profiles settings form.
 */
function simple_access_grants_profile_list()
{
  $form = array();
  // get an array of current profile names
  $profiles = config_get('simple_access_grants.settings', 'sag_profiles');
  $profilenames = array_keys($profiles);

  if (empty($profilenames)) {
    backdrop_set_message(t('You have not yet defined any access profiles.'));
    backdrop_goto('admin/config/content/simple_access_grants/profiles/add');
  }

  // build a table of current profiles with links to edit or delete a profile
  $form['profiles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Current profiles'),
    '#description' => t('Here is a list of the current profiles.<br>Each profile is a set of role-grant arrays specifying the grants to apply to a role.<br>You may add additional profiles or edit the ones listed here.<br> Note that a profile is intended to be applied to the nodes of one particular type, selected when applying the profile.'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#access' => t('assign profiles to nodes'),
  );

  foreach ($profilenames as $row) {
    $form['profiles'][$row]['name'] = array(
      '#markup' => $row,
    );
    $form['profiles'][$row]['number'] = array(
      '#markup' => $profiles[$row]['number'],
    );
    $form['profiles'][$row]['operations'] = array(
      '#markup' => l(t('edit'), 'admin/config/content/simple_access_grants/profiles/' . $row . '/edit') . '&nbsp;&nbsp;&nbsp; ' . l(t('delete'), 'admin/config/content/simple_access_grants/profiles/' . $row . '/delete')
    );
  }
  return $form;
}

/**
 * Simple Access Grants add profile form.
 * Used for 'add profile'
 */
function simple_access_grants_add_profile_form($form, $form_state)
{
  backdrop_set_title(t('Add a new profile'));

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Profile name'),
    '#description' => t('Enter a name for this new profile, then choose the grant settings for each role.'),
    '#default_value' => isset($profile['name']) ? $profile['name'] : '',
    '#access' => t('assign profiles to nodes'),
    '#required' => TRUE,
  );

  $form['access'] = array(
    '#tree' => TRUE,
    '#theme' => 'simple_access_grants_form',
  );

  // Get array of current roles excluding anonymous
  // Function simple_access_grants_form_row requires grant settings
  // These should be null for this case of a new profile.
  $grants = array('view' => 0, 'update' => 0, 'delete' => 0);
  $roles = user_roles($members_only = TRUE);
  foreach ($roles as $role => $roledata) {
    $form['access'][$role] = simple_access_grants_form_row($role, $grants);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}


/**
 * Define add profile form submit callback.
 */
function simple_access_grants_add_profile_form_submit($form, &$form_state)
{
  $data = $form_state['values'];
  $profile = $data['name'];
  $grants = $data['access'];
  foreach ($grants as $grant => $grantdata) {
    config_set('simple_access_grants.settings', "sag_profiles.$profile.roles.$grant", $grantdata);
  }

  // We also need to give a number to this profile
  $profiles = config_get('simple_access_grants.settings', 'sag_profiles');
  // if non-zero build array of existing profile numbers
  $pnmax = 0;
  if (isset($profiles)) {
    foreach ($profiles as $profile => $pdata) {
      $pnum = isset($pdata['number']) ? $pdata['number'] : 0;
      if ($pnum > $pnmax) {
        $pnmax = $pnum;
      }
    }
  }
  config_set('simple_access_grants.settings', "sag_profiles.$profile.number", $pnmax + 1);


  $form_state['redirect'] = 'admin/config/content/simple_access_grants/rebuild';
}


/**
 * Simple Access Grants edit profile form.
 * Used for editing a profile
 */
function simple_access_grants_edit_profile_form($form, $form_state, $profile)
{
  // Get array of current roles excluding anonymous
  $roles = user_roles($members_only = TRUE);
  // Get array of current settings for this profile
  $profile_grants = config_get('simple_access_grants.settings', "sag_profiles.$profile");
  $role_grants = $profile_grants['roles'];
  //build form to edit profile
  backdrop_set_title(t('Edit a profile'));

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Profile name'),
    '#default_value' => $profile,
    '#access' => t('assign profiles to nodes'),
    '#size' => 40,
    '#maxlength' => 80,
    '#description' => t('The name of this profile'),
    '#required' => TRUE,
  );

  $form['access'] = array(
    '#tree' => TRUE,
    '#theme' => 'simple_access_grants_form',
  );

  // list all roles and provide select boxes to choose grants for role
  foreach ($roles as $role) {
    // get current grant for this role to use as default value
    $role = strtolower($role);
    if (isset($role_grants[$role])) {
      $grant = $role_grants[$role];
    } else {
      $grant = 0;
    }
    $form['access'][$role] = simple_access_grants_form_row($role, $grant);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Define edit profile form submit callback.
 */
function simple_access_grants_edit_profile_form_submit($form, &$form_state)
{
  $data = $form_state['values'];
  $profile = $data['name'];
  $grants = $data['access'];
  foreach ($grants as $grant => $grantdata) {
    config_set('simple_access_grants.settings', "sag_profiles.$profile.roles.$grant", $grantdata);
  }

  $form_state['redirect'] = 'admin/config/content/simple_access_grants/rebuild';
}

/**
 * Simple Access apply profile form.
 */
function simple_access_grants_apply_profile_form()
{
  // form to select a content type and select a profile to apply to all nodes of that type
  backdrop_set_title(t('Apply a profile to a content type'));

  // use backdrop function to get node type names
  $nodetypelist = node_type_get_names();

  $form['nodetype'] = array(
    '#type' => 'radios',
    '#title' => t('Types of content'),
    '#default_value' => '',
    '#options' => $nodetypelist,
    '#access' => t('assign profiles to nodes'),
    '#description' => t('Choose one type of content and one profile.<br>This procedure applies the selected profile to the selected type of content.<br>A profile comprises grants for two or more roles.'),
  );

  // get list of profiles
  $profiles = config_get('simple_access_grants.settings', 'sag_profiles');
  $profilenames = array();
  foreach ($profiles as $pname => $pdata) {
    $profilenames[$pname] = $pname;
  }

  $form['profilename'] = array(
    '#type' => 'radios',
    '#title' => t('Available profiles'),
    '#default_value' => '',
    '#options' => $profilenames,
    '#description' => t('Choose which profile'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply this'),
  );
  return $form;
}

/**
 * Apply profile form submit callback.
 */
function simple_access_grants_apply_profile_form_submit($form, &$form_state)
{
  $selectednodetype = $form['nodetype']['#value'];
  $profilename = $form['profilename']['#value'];
  // now update config settings
  // add profile name to profiles in nodetype
  $nodetypeprofiles = array();
  $nodetypeprofiles = config_get('simple_access_grants.settings', "sag_nodetypes.$selectednodetype.profiles");
  // only add profile if not already included
  if (!in_array($profilename, $nodetypeprofiles)) {
    $nodetypeprofiles[] = $profilename;
    config_set('simple_access_grants.settings', "sag_nodetypes.$selectednodetype.profiles", $nodetypeprofiles);
  }

  backdrop_set_message(t('You now need to !rebuild_permissions.', array('!rebuild_permissions' => l(t('rebuild permissions'), 'admin/reports/status/rebuild'))));

  $form_state['redirect'] = 'admin/config/content/simple_access_grants/rebuild';
}

/**
 * Confirm profile delete form.
 */
function simple_access_grants_profile_delete_confirm($form, &$form_state, $profile)
{
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $profile,
    '#access' => t('assign profiles to nodes'),
  );
  return confirm_form($form,
    t('Are you sure you want to delete this profile?'),
    'admin/config/content/simple_access_grants/profiles',
    t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

/**
 * Confirm delete submit callback.
 */
function simple_access_grants_profile_delete_confirm_submit($form, &$form_state)
{
  $profile = $form_state['values']['pid'];
  // rebuild list of profiles excluding this one
  // get current config settings
  $profiles = config_get('simple_access_grants.settings', 'sag_profiles');
  $revisedprofiles = array();
  // build revised config settings
  foreach ($profiles as $pname => $pdata) {
    if ($pname != $profile) {
      $revisedprofiles[$pname] = $pdata;
    }
  }
  config_set('simple_access_grants.settings', 'sag_profiles', $revisedprofiles);

  // delete this profile from entries in config sag_nodetypes
  // check each nodetype
  $nodetypes = config_get('simple_access_grants.settings', "sag_nodetypes");

  foreach ($nodetypes as $ntype => $ndata) {
    $newprofiles = array();
    foreach ($ndata['profiles'] as $nprofile) {
      if ($nprofile != $profile) {
        $newprofiles[] = $nprofile;
      }
    }
    config_set('simple_access_grants.settings', "sag_nodetypes.$ntype.profiles", $newprofiles);
  }
  $form_state['redirect'] = 'admin/config/content/simple_access_grants/rebuild';
}


/**
 * Simple Access Grants settings form.
 */
function simple_access_grants_settings_page()
{
  backdrop_set_title(t('Simple Access Grants Settings'));
  $options = array(
    'rebuild' => t('<strong>Rebuild</strong>: Rebuild the Node Access permissions data table for the realm Simple Access Grant using the settings provided by this module.'),

  );

  $form['simple_access_showgroups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rebuild permissions'),
    '#description' => 'If ever the access settings for a node type are changed it is essential to rebuild the Node Access permissions data table for the realm Simple Access Grant',
  );

  // Add a submit button
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next step'),
  );

  return $form;
}

/**
 * Submit handler for module_settings_form().
 */
function simple_access_grants_settings_page_submit($form, &$form_state)
{

  $form['actions']['submit'] = NULL;

  backdrop_set_message(t('You now need to !rebuild_permissions.', array('!rebuild_permissions' => l(t('rebuild permissions'), 'admin/reports/status/rebuild'))));
}

