<?php
/**
 * Created by PhpStorm.
 * User: dg529
 * Date: 4/5/16
 * Time: 9:43 AM
 */

$items = $user_profile_links['#items'];

print $items['user_picture'];
?>

<h2>
    <?php echo $items['username'];?>
</h2>


<div class="item-list">

    <ul>

    <?php foreach($items['links'] as $item) { ?>

        <li class="first">
            <?php echo $item['link']; ?>
        </li>

    <?php } ?>

    </ul>

</div>
