<?php

/**
 * @file
 * Create the vocabulary for faq categories and add initial vocabulary terms.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the faq category vocabulary.
 * 
 */
function smartparticipation_core_create_faq_category_vocabulary($weight = 1) {
  smartparticipation_core_add_vocabulary('FAQ Categories', 'sp_faq_category',
    'Categories for FAQ content.', $weight);
  smartparticipation_core_add_faq_category_terms();
}

/**
 * Define the terms for the faq category vocabulary.
 */
function smartparticipation_core_add_faq_category_terms() {
  // Weight determines sort order in vocabulary terms list. Will also use to
  // sort the categories on the FAQs page.
  smartparticipation_core_create_term('sp_faq_category',
    'Site operation and relation to the official rulemaking process', 0);
  smartparticipation_core_create_term('sp_faq_category',
    'The who and what of site moderation', 1);
  smartparticipation_core_create_term('sp_faq_category',
    'Site use tips and help', 2);        
}

/** 
 * Delete the faq category vocabulary.
 */
function smartparticipation_core_delete_faq_category_vocabulary() {
  smartparticipation_core_delete_vocabulary('sp_faq_category');
}
