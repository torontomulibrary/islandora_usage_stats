islandora_usage_stats
=====================

A simple module for Drupal 7 to track views of islandora items

Work based on code from https://github.com/roblib/islandora_scholar_upei and the scholar_tracking module for Drupal 6

Creates new variables in the $variables array to allow templates to display view count and download count of Islandora items.

##Example without tracking:
```php
<div class="islandora-pdf-content-wrapper clearfix">
	<?php if (isset($islandora_content)): ?>
  <div class="islandora-pdf-content">
    <?php print $islandora_content; ?>
  </div>
  <?php if (isset($islandora_download_link)): ?>
  <?php print $islandora_download_link; ?>
    <?php endif; ?>
  <?php endif; ?>
</div>
```

##Example with tracking
```php
<div class="islandora-pdf-content-wrapper clearfix">
	<?php if (isset($islandora_content)): ?>
  <div class="islandora-pdf-content">
    <?php print $islandora_content; ?>
  </div>
  <?php if (isset($islandora_download_link)): ?>
  <?php print $islandora_download_link; ?>
    <?php endif; ?>
  <?php endif; ?>
  <?php if (module_exists('islandora_usage_stats')): ?>
  </br>
  <div>
    <p>
      Viewed: <span class="badge"><?php print $times_viewed ?></span> Last viewed: <?php print date('M d, Y',strtotime($time_last_viewed)); ?></span>
    </p>
    <p>
    Downloaded: <span class="badge"><?php print $times_downloaded ?></span>
    </p>
  </div>  
  <?php endif; ?>
</div>
```

##Variables available:
- times_viewed: How many times the item has been viewed
- time_last_viewed: The datetime of the last recorded view
- times_downloaded: How many times the item has been downloaded
- time_last_downloaded:  The datetime of the last download

##Features
- View count is based on IP and comes default with a 5 minute cooldown for repeated requests from the same IP
- Tracks views of all types of objects in the repository
- IP Exclusion list to prevent artifically inflating counts while testing/developing
