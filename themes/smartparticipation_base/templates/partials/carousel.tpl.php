<?php
/**
 * Template for a carousel for any collection of nodes. Currently used on topic
 * and document pages.
 *
 */

extract($carousel_data);

?>

<div class="row">

    <?php if ($carousel_prompt) : ?>
        <div class="span2 carousel-prompt">
            <?php echo render($carousel_prompt); ?>
        </div>
    <?php endif; ?>

    <div
        class="span10 proposal-topics <?php if ($carousel_prompt) {
            echo 'with-prompt';
        } ?>">
        <div class="topic carousel-container">

            <?php if (empty($nodes)) : ?>
                <p class="no-content"><?php echo $no_topics_message; ?></p>
            <?php else: ?>

                <div class="carousel-prev paging-link"></div>
                <div class="carousel-items">

                    <div class="rs-carousel module">
                        <ul class="rs-carousel-runner">
                            <?php
                            foreach ($nodes as $node) :

                                ?>
                                <li class="rs-carousel-item">
                                    <p class="title">
                                        <?php print $node['link']; ?>

                                        <?php if ($show_comment_count): ?>
                                            <span class="count">
                              <?php print $node['comments_number'];?><span class="icon-comment"></span>
                            </span>
                                            <?php if (!empty($recommended_comments_enabled)): ?>
                                            <span class="count">
                                                <?php print $node['comments_recommended'];?><span class="icon-recommended"></span>
                                            </span>
                                            <?php endif; ?>

                                        <?php endif; ?>

                                    </p>
                                </li>

                            <?php endforeach; ?>

                        </ul>
                    </div> <!-- .rs-carousel.module -->
                </div> <!-- .carousel-items -->
                <div class="carousel-next paging-link"></div>

            <?php endif; ?>
        </div> <!-- .topic.carousel-container -->
    </div> <!-- .proposal-topics -->
</div> <!-- .row -->
