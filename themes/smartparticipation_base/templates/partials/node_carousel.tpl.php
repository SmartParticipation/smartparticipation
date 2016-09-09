<?php

/**
 * Template for a carousel for any collection of nodes. Currently used on topic
 * and document pages.
 * 
 * TODO Combine with tabbed topic carousel markup in 
 * tabbed_topic_carousel.tpl.php. This template repeats the interior portion of 
 * that (except that the data here is compiled in the controller function 
 * rather than in the template, which is how it should be done).
 */

extract($carousel_data);

?>

<h3 id="all-topics"><?php echo $title; ?></h3>
<?php
echo render($carousel);
?>