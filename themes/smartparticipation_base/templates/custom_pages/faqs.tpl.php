<?php
/**
 * Template for SmartParticipation FAQ page.
 * 
 * Available variables:
 *  - $faq_categories: a structured array of faqs organized by category.
 * 
 * @see smartparticipation_core_faq_page()
 */

 
?>

<?php if (!empty($faq_categories['#items'])): ?>
  <?php foreach ($faq_categories['#items'] as $faq_category => $faqs): ?>
    <div class="faq-category">
      <h2><?php echo $faq_category; ?></h2>
      <ul>
        <?php foreach ($faqs as $faq): ?>
          <li>         
            <h3 class="faq-question">
              <a href="#">
                <span class="faq-question-icon"></span>
                <?php echo $faq['question']; ?>
              </a>
            </h3>
            <div class="faq-answer">
              <span class="faq-answer-icon"></span>
              <?php echo $faq['answer'];  ?>            
            </div>
          </li>
        <?php endforeach; ?>   
      </ul>  
    </div>
  <?php endforeach; ?>
  
<?php else: ?>
  <p class="no-content">There are currently no FAQs to display.</p> 
<?php endif; ?>
