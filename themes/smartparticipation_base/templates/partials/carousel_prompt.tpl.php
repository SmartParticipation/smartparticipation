<?php 

/**
 * Template for carousel prompt.
 * 
 * Available variables:
 * $prompt:
 *   - title 
 *   - text
 *   
 * See smartparticipation_carousel_prompt().
 */

// TODO - if possible, put if logic and surrounding div here rather than in
// calling template. Note that one is span1 and one is span2, though.
?>

<p title="<?php echo $prompt['title']; ?>"><?php echo $prompt['text']; ?></p>