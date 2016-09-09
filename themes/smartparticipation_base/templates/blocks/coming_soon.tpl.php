<?php
/**
 * @file 
 * Template for display of front page Coming Soon block.
 *
 * TODO This has now been customized for our custom animated image. We'll need
 * to allow users to add a custom template for the block, since we will not be
 * supplying this image and thus not this template.
 *   
 */

?>

<section id="coming-soon">  

  <div class="row">
    <div class="span12 section-heading">
      <h1><?php echo $coming_soon['title']; ?></h1>
              
      <?php if (!empty($coming_soon['#items'])) : ?>

      <?php else : ?>

      <div class="left"><img src="/sites/all/themes/regroom_base/images/coming-soon/coming-soon_01.png" width="351" height="250" alt=""></div>
    
      <div class="middle">
        
        <div class="rotation-1 rotate2"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0000_1.png"></div>
        <div class="rotation-2 rotate1"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0001_2.png"></div>
        <div class="rotation-3 rotate3"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0002_3.png"></div>
        <div class="rotation-4 rotate2"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0003_4.png"></div>
        
        <div class="rotation-5 rotate2"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0004_6.png"></div>
        
        <div class="rotation-6 rotate1"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0005_7.png"></div>
        <div class="rotation-7 rotate3"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0006_8.png"></div>
        <div class="rotation-8 rotate2"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0007_11.png"></div>
        
        <div class="rotation-9 rotate1"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0008_9.png"></div>
        
        <div class="rotation-10 rotate3"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0009_10.png"></div>
         
        <div class="rotation-11 rotate3"><img src="/sites/all/themes/regroom_base/images/coming-soon/Team_0010_5.png"></div>
        <div class="rotation-13 rotate4"><img src="/sites/all/themes/regroom_base/images/coming-soon/big-gear.png"></div>
        
        <div class="rotation-12"><img src="/sites/all/themes/regroom_base/images/coming-soon/logo.png"></div>

      </div>

      <div class="right"><img src="/sites/all/themes/regroom_base/images/coming-soon/coming-soon_03.png" width="318" height="250" alt=""></div>    

        <?php //echo render($coming_soon['image']); ?>

        <?php if ($coming_soon['text']) : ?>
          <div id="coming-soon-text"><?php echo render($coming_soon['text']); ?></div>
        <?php endif; ?>

        <?php if ($coming_soon['signup_link']) : ?>
          <p><?php echo $coming_soon['signup_link']; ?></p>
        <?php endif; ?>
      <?php endif; ?>          
    </div>
  </div>
</section>
