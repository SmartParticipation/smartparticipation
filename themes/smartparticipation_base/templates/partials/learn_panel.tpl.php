<?php
/**
 * @file 
 * Template for learn panel.
 */

?>

<?php if ( ! empty($panel['#items']) ) : ?>
  
  <section id="sp-learn">
    <div class="row">
      <div class="span12">
        <div id="learn-accordion" class="learn-items-<?php echo count($panel['#items']); ?>">
          <ol>
            <?php foreach ($panel['#items'] as $item) { ?>
              <li class="learn-item">
                <h2><span><?php echo $item['tab_title']; ?></span></h2>
                <div>
                  <figure>
                    <div class="learn-video-wrapper">
                      <?php if ($item['video']) { ?>
                        <div class="learn-video" id="<?php echo 'learn-video-' . $item['nid']; ?>" data-video="<?php echo $item['video']; ?>" data-image="<?php echo $item['image']; ?>"></div> 
                      <?php } else { ?>
                        <img src="<?php echo $item['image']; ?>" />
                      <?php } ?>
                    </div>
                    <figcaption>
                      <span class="title"><?php echo $item['title']; ?></span>
                      <span class="text"><?php echo $item['teaser']; ?></span>
                      <?php if (!$panel['single_node_view']): ?>
                        <span class="read-more-wrapper"><span class="read-more"><a href="<?php echo $item['url']; ?>"><?php echo t('Read More'); ?></a></span></span>
                      <?php endif; ?>
                    </figcaption>
                  </figure>
                </div>
              </li>
            <?php } ?>
          </ol>
          <noscript>
            <p><?php echo t('Please enable JavaScript to get the full experience.'); ?></p>
          </noscript>
     
        </div>
      </div>
    </div>
  </section>
  
<?php elseif (isset($panel['add_node_instructions']) && $panel['add_node_instructions']) : ?>

    <p><?php echo $panel['add_node_instructions']; ?></p>
    
<?php endif; ?>