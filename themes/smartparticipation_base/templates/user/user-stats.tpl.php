<?php
/**
 * Template for user stats page.
 */

//kpr($user_data);

extract($user_data['#items']);

?>

<h2><?php echo $registration_year; ?></h2>

<section class="profile-stats">
  <ul>
  
    <li>
      <p onclick="location.href='<?php echo $proposals['href']; ?>'">
        <span class="icon-label">
          <span class="proposals-participated-in"></span>
          <span class="count"><?php echo $proposals['count']; ?></span>
        </span>
        <span class="stats-red"><?php echo $proposals['link']; ?></span>
      </p>
    </li>
    
    <li>
      <p onclick="location.href='<?php echo $user_comments['href']; ?>'">
        <span class="icon-label">
          <span class="comments"></span>
          <span class="count"><?php echo $user_comments['count']; ?></span>
        </span>
        <span class="stats-orange "><?php echo $user_comments['link']; ?></span>
      </p>
    </li>
    
    <li>
      <p onclick="location.href='<?php echo $user_comments_endorsed['href']; ?>'">
        <span class="icon-label">
          <span class="comments-endorsed"></span>
          <span class="count"><?php echo $user_comments_endorsed['count']; ?></span>
        </span>
        <span class="stats-pink"><?php echo $user_comments_endorsed['link']; ?></span>
      </p>
    </li>

    <?php if ($recommended_comments_enabled) { ?>
    <li>
      <p onclick="location.href='<?php echo $user_comments_recommended['href']; ?>'">
        <span class="icon-label">
          <span class="recommended-comments"></span>
          <span class="count"><?php echo $user_comments_recommended['count']; ?></span>
        </span>
        <span class="stats-green"><?php echo $user_comments_recommended['link']; ?></span>
      </p>
    </li>
    <?php } ?>

    <li>
      <p onclick="location.href='<?php echo $replies_to_user_comments['href']; ?>'">
        <span class="icon-label">
          <span class="replies-to"></span>
          <span class="count"><?php echo $replies_to_user_comments['count']; ?></span>
        </span>
        <span class="stats-blue"><?php echo $replies_to_user_comments['link']; ?></span>
      </p>
    </li>
    
    <li>
      <p onclick="location.href='<?php echo $comments_user_endorsed['href']; ?>'">
        <span class="icon-label">
          <span class="endorsements-of"></span>
          <span class="count"><?php echo $comments_user_endorsed['count']; ?></span>
        </span>
        <span class="stats-brown"><?php echo $comments_user_endorsed['link']; ?></span>
      </p>
    </li>
    
  </ul>
</section>