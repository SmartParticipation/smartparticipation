<?php
/**
 * @file
 * Functions related to smartparticipation_core glossary.
 */


/**
 * Create the site glossary vocabulary.
 * 
 */
function smartparticipation_core_create_glossary_vocabulary() {
  $weight = smartparticipation_core_get_vocabulary_maximum_weight() + 1;
  smartparticipation_core_add_vocabulary('Site Glossary', 'sp_site_glossary', 'The site glossary.', $weight);
}


/** 
 * Delete the site glossary vocabulary.
 */
function smartparticipation_core_delete_glossary_vocabulary() {
  smartparticipation_core_delete_vocabulary('sp_site_glossary');
}


/**
 * Show the Glossary tab based on access permissions
 */
function smartparticipation_core_access_check_glossary($node) {

  if ($node->type == 'sp_proposal') {

    $vocabulary_machine_name = $machine_name = 'sp_proposal_glossary_' . $node->nid;
    $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_machine_name);
    $vid = $vocabulary->vid;
    
    // TODO Create a custom permission that is defined by these three options
    return user_access('administer taxonomy', $account = NULL) ||
           user_access('edit terms in ' .$vid, $account = NULL) ||
           user_access('delete terms in ' .$vid, $account = NULL);
  }
  
  return FALSE;

}


function smartparticipation_core_manage_glossary($node) {
  $path['destination'] = 'node/'.$node->nid;
  $admin_glossary_page = 'admin/structure/taxonomy/sp_proposal_glossary_' . $node->nid;
  drupal_goto($admin_glossary_page, array('query' => $path));
}


/**
 * Create a new glossary vocabulary when a proposal node is created.
 * 
 * @param $node - the proposal node object
 */
function smartparticipation_core_add_proposal_glossary($node) {
  
  $weight = smartparticipation_core_get_vocabulary_maximum_weight() + 1;
  $name = 'Glossary: ' . $node->title;
  $machine_name = 'sp_proposal_glossary_' . $node->nid;
  $description = 'Glossary for Proposal: ' . $node->title . '.';
  
  $vid = smartparticipation_core_add_vocabulary($name, $machine_name, $description, $weight);

  // Assign the permissions for this glossary vocabulary
  smartparticipation_core_assign_permissions_glossary($vid);
}


/**
 * Update the proposal glossary name and description after the proposal is 
 * modified.
 * 
 * @param $proposal - the proposal node object
 */
function smartparticipation_core_update_proposal_glossary($proposal) {
  
  $title_new = $proposal->title;
  
  if ($title_new != $proposal->original->title) {

    $glossary = taxonomy_vocabulary_machine_name_load('sp_proposal_glossary_' . $proposal->nid);
    $glossary->name = 'Glossary: '  . $title_new;
    $glossary->description = 'Glossary for Proposal: ' . $title_new;
    taxonomy_vocabulary_save($glossary);
    
  }
}


/** 
 * Delete a proposal glossary vocabulary.
 * 
 * @param $proposal - the proposal node object
 */
function smartparticipation_core_delete_proposal_glossary($proposal) {
  $machine_name = 'sp_proposal_glossary_' . $proposal->nid;
  $glossary = taxonomy_vocabulary_machine_name_load($machine_name);
  
  // revoke the permission for this vocabulary
  smartparticipation_core_revoke_permissions_glossary($glossary->vid);
  // delete the vocabulary
  smartparticipation_core_delete_vocabulary($machine_name);
}


/** 
 * Delete all the proposal glossary vocabularies.
 */
function smartparticipation_core_delete_proposal_glossary_vocabulary_all() {
  $proposals = _smartparticipation_core_get_proposal_list_all();
  
  foreach($proposals as $nid => $title) {
    smartparticipation_core_delete_vocabulary('sp_proposal_glossary_' . $nid);
  }
}

/**
 * Build the links to the glossary terms
 * 
 * @param $text - the text in which glossary abbreviations are to be inserted
 * @param $glossary_data - a structured array of glossary terms
 * 
 * @return $text with glossary definitions inserted as abbr elements
 * 
 */
function smartparticipation_core_insert_glossary_definitions($text, $glossary_data){
  /*
   * First remove non-breaking spaces added by TinyMCE. The non-breaking spaces
   * screw up the behavior of the bootstrap glossary tooltip: if there's a long
   * stretch of non-breaking spaces around a glossary term, the text starts
   * jumping around when the tooltip is displayed. Also, we are now doing
   * negative lookarounds for non-breaking spaces, so they prevent term
   * matching.
   */
  $text = preg_replace('/\xC2\xA0/',' ', $text);
  $text = str_replace('&nbsp;', ' ', $text);

  $max_definition_length = $glossary_data['max_definition_length'];
  $terms = $glossary_data['terms'];

    foreach ($terms as $term=>$definition) {

      /*
       * Check for word boundaries on either side of the term.
       *
       * Negative lookahead (?![^<]{0,$max_definition_length}>) blocks expansion
       * inside an HTML tag (e.g., a title attribute - not content inside an
       * opening and  closing tag, of course). Use $max_definition_length so we
       * look ahead only as far as necessary.
       *
       * Because we insert non-breaking spaces into the term, we can use this to
       * prevent expansion of all subterms (both leftward and rightward), by
       * doing negative lookahed and lookbehind for a non-breaking space.
       * Examples:
       * digital divide" => "digital&nbsp;divide", so term "digital" won't be
       * expanded due to negative lookahead
       * "small servicer" => "small&nbsp;servicer", so term "servicer" won't be
       * expanded due to negative lookbehind
       */

      // If there is a period at the end of a term remove it but include it
      // as part of the search and include in parentheses
      if (preg_match("/\.$/",$term)){
        $term = preg_replace("/\.$/","",$term);
        $search_string = "/(?<!&nbsp;)\b(" . $term . "\b\.)(?![^<]{0," . $max_definition_length . "}>)(?!&nbsp;)/";
      }
      else {
        $search_string = "/(?<!&nbsp;)\b(" . $term . ")\b(?![^<]{0," . $max_definition_length . "}>)(?!&nbsp;)/";
      }

      /* If the term is all lowercase, do a case-insensitive search; otherwise, do
       * a case-sensitive search.
       *
       * This prevents WHO => World Health Organization matching "who,"
       * though it will not prevent all case-related over- and under-matching
       * errors.
       *
       * Note that "health IT" should be added instead of "health it", to prevent
       * over-matching on "if you are in good health it means...". This means
       * "Health IT" must also be added.
       * TODO Match initial uppercase even if term has initial lowercase.
       *
       * Don't test ctype_lower($term), which returns false on "self-management".
       */
      if (strtolower($term) == $term) {
        $search_string .= 'i';
      }

      $text = preg_replace($search_string,
          /*
           * Insert non-breaking spaces within the glossary term, since the
           * tooltip placement is messed up if the term breaks over a line. This
           * also aids the replacement - see above.
           */
          '<abbr class="glossary" title="' . htmlentities($definition, ENT_QUOTES) . '">' . str_replace(' ', '&nbsp;', "\\1") . '</abbr>',
          $text);

    }


  return $text;
}

/**
 * Apply glossary lookup to text.
 * 
 * @param $text - the text to apply glossary lookup to
 * @param $proposal_nid (optional) - the associated proposal nid. If non-null, 
 * the proposal glossary will be applied; else the site glossary.
 * 
 * @return the text with glossary definitions applied
 */
function smartparticipation_core_apply_glossary_lookup($text, $proposal_nid = NULL) {
  
  // TODO (maybe) Apply sitewide glossary after proposal glossary too. Do this
  // by combining the $glossary_data into one large array, rather than applying 
  // first one glossary, then the other. That would cause problems if they have
  // common terms. So here we would merge the proposal glossary data with
  // the site glossary data, with the first taking precedence. We would use 
  // whichever value of max_definition_length is largets.
  
  $glossary_data = $proposal_nid ? 
                   smartparticipation_core_get_proposal_glossary_data($proposal_nid) :
                   smartparticipation_core_get_site_glossary_data();
                    
  $text = smartparticipation_core_insert_glossary_definitions($text, $glossary_data);
  return $text;
}

/**
 * Return a structured array of site glossary data. 
 * 
 * @return array 
 * - terms: an array mapping term names to definitions
 * - max_definition_length: the length of the longest definition
 * 
 */
function smartparticipation_core_get_site_glossary_data() {

  $vocabulary_name = 'sp_site_glossary';
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
  return smartparticipation_core_get_glossary_data($vocabulary->vid);
  
}

/**
 * Return a structured array of proposal glossary data. 
 * 
 * @param $proposal_nid - the nid of the proposal
 * 
 * @return array 
 * - terms: an array mapping term names to definitions
 * - max_definition_length: the length of the longest definition
 * 
 */
function smartparticipation_core_get_proposal_glossary_data($proposal_nid) {

  $vocabulary_name = 'sp_proposal_glossary_' . $proposal_nid;
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
  return smartparticipation_core_get_glossary_data($vocabulary->vid);
  
}

/**
 * Fetch glossary data from the database and return as a structured array.
 * 
 * @param $vid - the vocabulary id
 * 
 * @return array 
 * - terms: an array mapping term names to definitions
 * - max_definition_length: the length of the longest definition
 */
function smartparticipation_core_get_glossary_data($vid) {
  
  $terms = smartparticipation_core_get_glossary_terms($vid);
  $max_definition_length = smartparticipation_core_get_max_definition_length($terms);
  
  $glossary_data = array(
    'terms' => $terms,
    'max_definition_length' => $max_definition_length,
  );
  
  return $glossary_data;
}


/**
 * Get all the terms from one vocabulary ordered by term length
 * 
 * @param vid - vocabulary vid
 * 
 * @return an array mapping glossary terms to their definitions
 */
function smartparticipation_core_get_glossary_terms($vid) {
  
  // Fetch terms
  $query = "SELECT tid, name, description
            FROM {taxonomy_term_data} 
            WHERE vid = :vid
            ORDER BY LENGTH(name) DESC";
  
  $result = db_query($query, array(
      ':vid' => $vid
      ))
  ->fetchAllAssoc('tid', PDO::FETCH_ASSOC);
  
  $terms = array();
  
  foreach ($result as $tid => $value) {
    $terms[$value['name']] = $value['description'];
  }
  
  return $terms;
 
}

/**
 * Return the length of the longest glossary definition in a set of terms.
 * 
 * @param $terms - array mapping term names to term descriptions
 * 
 * @return integer - the length of the longest definition in $terms
 */
function smartparticipation_core_get_max_definition_length($terms) {
    
  /* Is it faster to iterate through the fetched data, or do another SQL query?
  $query = "SELECT MAX(LENGTH(description)) 
            FROM {taxonomy_term_data}
            WHERE vid = :vid";
            
  $max_definition_length = db_query($query, array(
      ':vid' => $vid
    )) 
    ->fetchField();
  */
  
  $max_definition_length = 0;
  foreach ($terms as $term=>$definition) {
    $definition_length = strlen($definition);
    if ($definition_length > $max_definition_length) {
      $max_definition_length = $definition_length;
    }
  }
  
  return $max_definition_length;
   
}



/** 
 * Customize the glossary vocabulary term form.
 */
function smartparticipation_core_glossary_vocabulary_form_term_alter(&$form, &$form_state, $form_id) {
        
  $form['name']['#title'] = t('Term');
  $form['name']['#description'] = t('Enter the term in all lowercase unless the term lookup should be case-sensitive. 
      For example, enter "WHO" for "World Health Organization" so that it does not apply to "who."');
  $form['description']['#title'] = t('Definition');
  $form['relations']['#access'] = FALSE; 
  
}

/**
 * Customize the glossary overview form.
 */
function smartparticipation_core_form_glossary_overview_terms_alter(&$form, &$form_state, $form_id) {
  // Add custom theme wrapper to glossary term overview
  array_unshift($form['#theme'], 'sp_taxonomy_overview_terms');
}
    
/**
 * Return true iff the vocabulary is a smartparticipation_core glossary.
 */
function smartparticipation_core_is_glossary_vocabulary($vocabulary_machine_name) {
  return $vocabulary_machine_name == 'sp_site_glossary' ||
  strstr($vocabulary_machine_name, 'sp_proposal_glossary_');
}
