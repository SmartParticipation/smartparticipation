<?php

/**
 * @file
 * Implements the basic functionality required to create and display an entity.
 */


/**
 * Called from smartparticipation_core_entity_info() to provide the info for this entity type.
 */
function smartparticipation_core_sample_entity_info() {
  $info['smartparticipation_core_sample'] = array(
    // A human readable label to identify our entity.
    'label' => t('RegRoom Sample Entity'),

    // The controller for our Entity, extending the Drupal core controller.
    'controller class' => 'RegRoomSampleController',

    // The table for this entity defined in hook_schema()
    'base table' => 'smartparticipation_core_sample',

    // Returns the uri elements of an entity
    'uri callback' => 'smartparticipation_core_sample_uri',

    // IF fieldable == FALSE, we can't attach fields.
    'fieldable' => TRUE,

    // entity_keys tells the controller what database fields are used for key
    // functions. It is not required if we don't have bundles or revisions.
    // Here we do not support a revision, so that entity key is omitted.
    'entity keys' => array(
      'id' => 'sample_id' , // The 'id' (sample_id here) is the unique id.
      'bundle' => 'bundle_type' 
    ),

    'bundle keys' => array(
      'bundle' => 'bundle_type',
    ),

    // FALSE disables caching. Caching functionality is handled by Drupal core.
    'static cache' => TRUE,

    // Bundles are alternative groups of fields or configuration
    // associated with a base entity type. This is needed even if the entity
    // exposes only one bundle.
    'bundles' => array(
      'first_sample_bundle' => array(
        'label' => 'First sample bundle',
        // 'admin' key is used by the Field UI to provide field and
        // display UI pages.
        'admin' => array(
          'path' => 'admin/structure/smartparticipation_core_sample/manage',
          'access arguments' => array('administer smartparticipation_core_sample entities'),
        ),
      ),
    ),
    
    // View modes allow entities to be displayed differently based on context.
    // As a demonstration we'll support "Basic", but we could have and support
    // multiple display modes.
    'view modes' => array(
      'basic' => array(
        'label' => t('Basic'),
        'custom settings' =>  FALSE,
      ),
    )
  );

  return $info;
}
/**
 * Fetch a sample object.
 *
 * This function ends up being a shim between the menu system and
 * smartparticipation_core_sample_load_multiple().
 *
 * This function gets its name from the menu system's wildcard
 * naming conventions. For example, /path/%wildcard would end
 * up calling wildcard_load(%wildcard value). In our case defining
 * the path: smartparticipation_core/sample/%smartparticipation_core_sample in
 * hook_menu() tells Drupal to call smartparticipation_core_sample_load().
 *
 * @param $sample_id
 *   Integer specifying the sample entity id.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   A fully-loaded $sample object or FALSE if it cannot be loaded.
 *
 * @see smartparticipation_core_sample_load_multiple()
 * @see smartparticipation_core_menu()
 */
function smartparticipation_core_sample_load($sample_id = NULL, $reset = FALSE) {
  $sample_ids = (isset($sample_id) ? array($sample_id) : array());
  $sample = smartparticipation_core_sample_load_multiple($sample_ids, array(), $reset);
  return $sample ? reset($sample) : FALSE;
}

/**
 * Loads multiple sample entities.
 *
 * We only need to pass this request along to entity_load(), which
 * will in turn call the load() method of our entity controller class.
 */
function smartparticipation_core_sample_load_multiple($sample_ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('smartparticipation_core_sample', $sample_ids, $conditions, $reset);
}

/**
 * Implements the uri callback.
 */
function smartparticipation_core_sample_uri($sample) {
  return array(
    'path' => 'smartparticipation_core/sample/' . $sample->sample_id,
  );
}

/**
 * Menu items for this entity.
 */
function smartparticipation_core_sample_menu_items() {
  $items['smartparticipation_core/sample'] = array(
    'title' => 'RegRoom Sample',
    'page callback' => 'smartparticipation_core_info_page',
    'access arguments' => array('view any smartparticipation_core_sample entity'),
  );

  // This provides a place for Field API to hang its own
  // interface and has to be the same as what was defined
  // in sample_entity_info() above.
  $items['admin/structure/smartparticipation_core_sample/manage'] = array(
    'title' => 'Manage RegRoom Sample entities',
    'page callback' => 'smartparticipation_core_sample_admin_page',
    'access arguments' => array('administer smartparticipation_core_sample entities'),
  );

  // The page to view our entities - needs to follow what
  // is defined in sample_uri and will use load_sample to retrieve
  // the necessary entity info.
  $items['smartparticipation_core/sample/%smartparticipation_core_sample'] = array(
    'title callback' => 'smartparticipation_core_sample_title',
    // NB The arguments array size corresponds to the number of segments in the
    // path. When this changed from examples/smartparticipation_core/sample to smartparticipation_core/sample,
    // the array size of the arguments must change from 3 to 2.
    'title arguments' => array(2),
    'page callback' => 'smartparticipation_core_sample_view',
    'page arguments' => array(2),
    'access arguments' => array('view any smartparticipation_core_sample entity'),
  );

  // 'View' tab for an individual entity page.
  $items['smartparticipation_core/sample/%smartparticipation_core_sample/view'] = array(
    'title' => 'View',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );

  // 'Edit' tab for an individual entity page.
  $items['smartparticipation_core/sample/%smartparticipation_core_sample/edit'] = array(
    'title' => 'Edit',
    'page callback' => 'drupal_get_form',
    // NB The arguments array size corresponds to the number of segments in the
    // path. When this changed from examples/smartparticipation_core/sample to smartparticipation_core/sample,
    // the array size of the arguments must change from 3 to 2.
    'page arguments' => array('smartparticipation_core_sample_form', 2),
    'access arguments' => array('edit any smartparticipation_core_sample entity'),
    'type' => MENU_LOCAL_TASK,
  );

  // Add example entities.
  $items['smartparticipation_core/sample/add'] = array(
    'title' => 'Add a RegRoom Sample entity',
    'page callback' => 'smartparticipation_core_sample_add',
    'access arguments' => array('create smartparticipation_core_sample entities'),
  );

  return $items;
}

/**
 * Sample information for the page.
 */
function smartparticipation_core_info_page() {
  $content['preface'] = array(
    '#type' => 'item',
    '#markup' => t('Provides a simple example entity.')
  );
  if (user_access('administer smartparticipation_core_sample entities')) {
    $content['preface']['#markup'] =  t('You can administer these and add fields and change the view !link.',
      array('!link' => l(t('here'), 'admin/structure/smartparticipation_core_sample/manage'))
    );
  }
  $content['table'] = smartparticipation_core_sample_list_entities();

  return $content;
}

/**
 * Implements hook_permission().
 */
function smartparticipation_core_sample_permissions() {
  $permissions = array(
    'administer smartparticipation_core_sample entities' =>  array(
      'title' => t('Administer RegRoom Sample entities'),
    ),
    'view any smartparticipation_core_sample entity' => array(
      'title' => t('View any RegRoom Sample entity'),
    ),
    'edit any smartparticipation_core_sample entity' => array(
      'title' => t('Edit any RegRoom Sample entity'),
    ),
    'create smartparticipation_core_sample entities' => array(
      'title' => t('Create RegRoom Sample entities'),
    ),
  );
  return $permissions;
}


/**
 * Provides a list of existing entities and the ability to add more. Tabs
 * provide field and display management.
 */
function smartparticipation_core_sample_admin_page() {
  $content = array();
  $content[] = array(
    '#type' => 'item',
    '#markup' => t('Administration page for RegRoom Sample entities.')
  );

  $content[] = array(
    '#type' => 'item',
    '#markup' => l(t('Add a RegRoom Sample entity'), 'smartparticipation_core/sample/add'),
  );

  $content['table'] = smartparticipation_core_sample_list_entities();

  return $content;
}

/**
 * Returns a render array with all smartparticipation_core_sample entities.
 *
 * In this basic example we know that there won't be many entities,
 * so we'll just load them all for display. See pager_example.module
 * to implement a pager. Most implementations would probably do this
 * with the contrib Entity API module, or a view using views module,
 * but we avoid using non-core features in the Examples project.
 *
 * @see pager_example.module
 */
function smartparticipation_core_sample_list_entities() {
  $content = array();
  // Load all of our entities.
  $entities = smartparticipation_core_sample_load_multiple();
  if (!empty($entities)) {
    foreach ( $entities as $entity ) {
      // Create tabular rows for our entities.
      $rows[] = array(
        'data' => array(
          'id' => $entity->sample_id,
          'item_description' => l($entity->item_description, 'smartparticipation_core/sample/' . $entity->sample_id),
          // 'bundle' => $entity->bundle_type,
        ),
      );
    }
    // Put our entities into a themed table. See theme_table() for details.
    $content['entity_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => array(t('ID'), t('Item Description')),
      // '#header' => array(t('ID'), t('Item Description'), t('Bundle')),
    );
  }
  else {
    // There were no entities. Tell the user.
    $content[] = array(
      '#type' => 'item',
      '#markup' => t('There are currently no RegRoom Sample entities.'),
    );
  }
  return $content;
}

/**
 * Callback for a page title when this entity is displayed.
 */
function smartparticipation_core_sample_title($entity) {
  return t('RegRoom Sample (item_description=@item_description)', array('@item_description' => $entity->item_description));
}

/**
 * Menu callback to display an entity.
 *
 * As we load the entity for display, we're responsible for invoking a number
 * of hooks in their proper order.
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 * @see hook_entity_view_alter()
 */
function smartparticipation_core_sample_view($entity, $view_mode = 'basic') {
  // Our entity type, for convenience.
  $entity_type = 'smartparticipation_core_sample';
  // Start setting up the content.
  $entity->content = array(
    '#view_mode' => $view_mode,
  );
  // Build fields content - this is where the Field API really comes in to play.
  // The task has very little code here because it all gets taken care of by
  // field module.
  // field_attach_prepare_view() lets the fields load any data they need
  // before viewing.
  field_attach_prepare_view($entity_type, array($entity->sample_id => $entity),
    $view_mode);
  // We call entity_prepare_view() so it can invoke hook_entity_prepare_view()
  // for us.
  entity_prepare_view($entity_type, array($entity->sample_id => $entity));
  // Now field_attach_view() generates the content for the fields.
  $entity->content += field_attach_view($entity_type, $entity, $view_mode);

  // OK, Field API done, now we can set up some of our own data.
  $entity->content['created'] = array(
    '#type' => 'item',
    '#title' => t('Created date'),
    '#markup' => format_date($entity->created),
  );
  $entity->content['item_description'] = array(
    '#type' => 'item',
    '#title' => t('Item Description'),
    '#markup' => $entity->item_description,
  );

  // Now to invoke some hooks. We need the language code for
  // hook_entity_view(), so let's get that.
  global $language ;
  $langcode = $language->language ;
  // And now invoke hook_entity_view().
  module_invoke_all('entity_view', $entity, $entity_type, $view_mode,
    $langcode);
  // Now invoke hook_entity_view_alter().
  drupal_alter(array('smartparticipation_core_sample_view', 'entity_view'),
    $entity->content, $entity_type);

  // And finally return the content.
  return $entity->content;
}

function smartparticipation_core_sample_extra_fields() {
  
   $form_elements['item_description'] = array(
    'label' => t('Item Description'),
    'description' => t('Item Description (an extra form field)'),
    'weight' => -5,
  );
  
  $display_elements['created'] = array(
    'label' => t('Creation date'),
    'description' => t('Creation date (an extra display field)'),
    'weight' => 0,
  );
  $display_elements['item_description'] = array(
    'label' => t('Item Description'),
    'description' => t('Just like title, but trying to point out that it is a separate property'),
    'weight' => 0,
  );

  // Since we have only one bundle type, we'll just provide the extra_fields
  // for it here.
  $extra_fields['smartparticipation_core_sample']['first_sample_bundle']['form'] = $form_elements;
  $extra_fields['smartparticipation_core_sample']['first_sample_bundle']['display'] = $display_elements;
  
  return $extra_fields;
}

/**
 * Provides a wrapper on the edit form to add a new entity.
 */
function smartparticipation_core_sample_add() {
  // Create a sample entity structure to be used and passed to the validation
  // and submission functions.
  $entity = entity_get_controller('smartparticipation_core_sample')->create();
  return drupal_get_form('smartparticipation_core_sample_form', $entity);
}

/**
 * Form function to create an smartparticipation_core_sample entity.
 *
 * The pattern is:
 * - Set up the form for the data that is specific to your
 *   entity: the columns of your base table.
 * - Call on the Field API to pull in the form elements
 *   for fields attached to the entity.
 */
function smartparticipation_core_sample_form($form, &$form_state, $entity) {
  $form['item_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Item Description'),
    '#required' => TRUE,
    '#default_value' => $entity->item_description,
  );

  $form['sample_entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );

  field_attach_form('smartparticipation_core_sample', $entity, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('smartparticipation_core_sample_edit_delete'),
    '#weight' => 200,
  );

  return $form;
}


/**
 * Validation handler for smartparticipation_core_sample_add_form form.
 * We pass things straight through to the Field API to handle validation
 * of the attached fields.
 */
function smartparticipation_core_sample_form_validate($form, &$form_state) {
  field_attach_form_validate('smartparticipation_core_sample', $form_state['values']['sample_entity'], $form, $form_state);
}


/**
 * Form submit handler: submits sample_add_form information
 */
function smartparticipation_core_sample_form_submit($form, &$form_state) {
  $entity = $form_state['values']['sample_entity'];
  $entity->item_description = $form_state['values']['item_description'];
  field_attach_submit('smartparticipation_core_sample', $entity, $form, $form_state);
  $entity = smartparticipation_core_sample_save($entity);
  $form_state['redirect'] = 'smartparticipation_core/sample/' . $entity->sample_id;
}

/**
 * Form deletion handler.
 *
 * @todo: 'Are you sure?' message.
 */
function smartparticipation_core_sample_edit_delete( $form , &$form_state ) {
  $entity = $form_state['values']['sample_entity'];
  smartparticipation_core_sample_delete($entity);
  drupal_set_message(t('The RegRoom Sample entity %item_description (ID %id) has been deleted',
    array('%item_description' => $entity->item_description, '%id' => $entity->sample_id))
  );
  $form_state['redirect'] = 'smartparticipation_core/sample';
}

/**
 * We save the entity by calling the controller.
 */
function smartparticipation_core_sample_save(&$entity) {
  return entity_get_controller('smartparticipation_core_sample')->save($entity);
}

/**
 * Use the controller to delete the entity.
 */
function smartparticipation_core_sample_delete($entity) {
  entity_get_controller('smartparticipation_core_sample')->delete($entity);
}

/**
 * RegRoomSampleControllerInterface definition.
 *
 * We create an interface here because anyone could come along and
 * use hook_entity_info_alter() to change our controller class.
 * We want to let them know what methods our class needs in order
 * to function with the rest of the module, so here's a handy list.
 *
 * @see hook_entity_info_alter()
 */

interface RegRoomSampleControllerInterface
  extends DrupalEntityControllerInterface {
  public function create();
  public function save($entity);
  public function delete($entity);
}

/**
 * RegRoomSampleController extends DrupalDefaultEntityController.
 *
 * Our subclass of DrupalDefaultEntityController lets us add a few
 * important create, update, and delete methods.
 */
class RegRoomSampleController
  extends DrupalDefaultEntityController
  implements RegRoomSampleControllerInterface {

  /**
   * Create and return a new smartparticipation_core_sample entity.
   */
  public function create() {
    $entity = new stdClass();
    $entity->type = 'smartparticipation_core_sample';
    $entity->sample_id = 0;
    $entity->bundle_type = 'first_sample_bundle';
    $entity->item_description = '';
    return $entity;
  }

  /**
   * Saves the custom fields using drupal_write_record()
   */
  public function save($entity) {
    // If our entity has no sample_id, then we need to give it a
    // time of creation.
    if (empty($entity->sample_id)) {
      $entity->created = time();
    }
    // Invoke hook_entity_presave().
    module_invoke_all('entity_presave', $entity, 'smartparticipation_core_sample');
    // The 'primary_keys' argument determines whether this will be an insert
    // or an update. So if the entity already has an ID, we'll specify
    // sample_id as the key.
    $primary_keys = $entity->sample_id ? 'sample_id' : array();
    // Write out the entity record.
    drupal_write_record('smartparticipation_core_sample', $entity, $primary_keys);
    // We're going to invoke either hook_entity_update() or
    // hook_entity_insert(), depending on whether or not this is a
    // new entity. We'll just store the name of hook_entity_insert()
    // and change it if we need to.
    $invocation = 'entity_insert';
    // Now we need to either insert or update the fields which are
    // attached to this entity. We use the same primary_keys logic
    // to determine whether to update or insert, and which hook we
    // need to invoke.
    if (empty($primary_keys)) {
      field_attach_insert('smartparticipation_core_sample', $entity);
    }
    else {
      field_attach_update('smartparticipation_core_sample', $entity);
      $invocation = 'entity_update';
    }
    // Invoke either hook_entity_update() or hook_entity_insert().
    module_invoke_all($invocation, $entity, 'smartparticipation_core_sample');
    return $entity;
  }

  /**
   * Delete a single entity.
   *
   * Really a convenience function for delete_multiple().
   */
  public function delete($entity) {
    $this->delete_multiple(array($entity));
  }

  /**
   * Delete one or more smartparticipation_core_sample entities.
   *
   * Deletion is unfortunately not supported in the base
   * DrupalDefaultEntityController class.
   *
   * @param $sample_ids
   *   An array of entity IDs or a single numeric ID.
   */
  public function delete_multiple($entities) {
    $sample_ids = array();
    if (!empty($entities)) {
      $transaction = db_transaction();
      try {
        foreach ($entities as $entity) {
          // Invoke hook_entity_delete().
          module_invoke_all('entity_delete', $entity, 'smartparticipation_core_sample');
          field_attach_delete('smartparticipation_core_sample', $entity);
          $sample_ids[] = $entity->sample_id;
        }
        db_delete('smartparticipation_core_sample')
          ->condition('sample_id', $sample_ids, 'IN')
          ->execute();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('smartparticipation_core', $e);
        throw $e;
      }
    }
  }
}
