<p><?=t('The closed @proposal_type you selected was done on an earlier version of the @site_name site.',
    array('@proposal_type'=>t(variable_get('smartparticipation_site_proposal_type')),'@site_name'=>variable_get('site_name')))?></p>
<p><?=t('If you choose to continue, you will be redirected to the archived site.')?></p>
<div class="actions" style="text-align: center">
<button class="continue"><?=t('Continue to the archived site')?></button>
<p><a href="#" class="cancel"><?=t('Cancel, I want to stay here.')?></a></p>
</div>

