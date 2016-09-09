<?php

$items = $about_links['#items'];

?>

<nav id="about-pages">

    <ul>

    <?php foreach($items['links'] as $item) { ?>

        <li>
            <?php echo $item; ?>
        </li>

    <?php } ?>

    </ul>

</nav>
