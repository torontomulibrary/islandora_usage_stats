Islandora Usage Stats
=====================

A simple module for Drupal 7 to track views of islandora items in the Ryerson Digital Repository.

Work based on code from https://github.com/roblib/islandora_scholar_upei and the scholar_tracking module for Drupal 6.

View our fancy page here!
http://ryersonlibrary.github.io/islandora_usage_stats

##Features
- Toggle to ignore common bots
- View count uses session variables and defaults to a 5 minute cooldown for repeated requests
- Tracks views of all types of objects in the repository
- Access log for all views and downloads, can be disabled
- IP Exclusion list to prevent artifically inflating counts while testing/developing
- Drupal hook to allow other modules to fire events based on datastream download (hook_islandora_datastream_download)
- Custom block to display most viewed items
- Permissions to view usage data on an item page
- Permissions to view most popular items block

##Variables available:
- **times_viewed:** How many times the item has been viewed
- **time_last_viewed:** The datetime of the last recorded view in seconds since the unix epoch, format with a call to date()
- **times_downloaded:** How many times the item has been downloaded
- **time_last_downloaded:**  The datetime of the last download in seconds since the unix epoch, format with a call to date()
- *islandora_download_link:* Altered for the Islandora PDF module only. Redirects the download request to allow tracking of PDF downloads

##How to use:
- Place the downloaded module in your sites/all/modules folder and enable it
- Configure any excluded IP addresses under Admin > Config > Islandora Usage Stats
- Make sure user permissions are set appropriately under People > Permissions
- Edit your site theme file to output the stats where you want them
- *Optional*: use the hook_islandora_datastream_download to add functionality based on a datastream download


##How to display tracking data on your template files:
- In order to display tracking data for Islandora objects we need to modify your site's theme files
- Add a function with the naming convention ```islandora_usage_stats_preprocess_yourTemplateFileName``` at the end of the islandora_usage_stats.module file (and replace ```yourTemplateFileName``` with the name of a template you want to override).
  - The function ```islandora_usage_stats_preprocess_islandora_pdf``` included in the ```islandora_usage_stats.module``` file can be used as an example and should be duplicated and renamed to affect other templates
- Copy the template file you want to override to your theme's ```templates``` directory and follow the example below to display tracking information

##An example:
We will add view and download stats to items in our Islandora PDF collection
###Step 1
Navigate to your Drupal site's modules directory and clone the latest version of the module

```$ cd <your_Drupal_Site>/sites/all/modules```

```$ git clone https://github.com/ryersonlibrary/islandora_usage_stats```

###Step 2
Open the ```islandora_usage_stats.module``` file in a text editor and create/edit the template preprocess function necessary to modify the Islandora PDF collection

```$ vim islandora_usage_stats/islandora_usage_stats.module```

The template preprocess function follows the naming convention of ```islandora_usage_stats_preprocess_MODULE_NAME(&$variables)``` so make sure to edit yours accordingly

```php
function islandora_usage_stats_preprocess_islandora_pdf($&variables)
{
  $islandora_object = $variables['islandora_object'];
  $id = $islandora_object->id;
  $label = $islandora_object->label;
  
  $track = new IslandoraUsageStats($id);
  $variables['time_last_viewed'] = $track->getLastViewedTime();
  
  $currentStats = $track->getViewCount();
  $variables['times_viewed'] = $currentStats['views'];
  
  $downloaded = $track->getDownloads('OBJ');
  
  $variables['times_downloaded'] = $downloaded['downloads'];
  $variables['time_last_downloaded'] = $downloaded['time'];
  
  // we have added some bootstrap specific css classes and glyphicons to our link, include if desired
  $variables['islandora_download_link'] = l('<i class="glyphicon glyphicon-download"> ' . t('Download PDF') . '</i>', 'download_ds/' . $id . '/OBJ/' . $label, array(
    'attributes' => array(
          'class' => 'btn btn-primary'
      ),
      'html' => TRUE,
  ));
}

```
###Step 3
Navigate to your site's themes directory and copy the Islandora PDF template file so we can make changes to it and override it

```$ cd <your_Drupal_Site>/sites/themes/<yourtheme>```

```$ cp <your_Drupal_Site>/sites/all/islandora_solution_pack_pdf/theme/islandora-pdf.tpl.php .```

###Step 4
Edit the copied template file to output our new tracking variables 
####Before
**/var/www/your_Drupal_Site/sites/themes/your_Theme/islandora-pdf.tpl.php**
```html
<!------- SNIP ---------->
<div class="islandora-pdf-content-wrapper clearfix">
<?php if (isset($islandora_content)): ?>
  
  <div class="islandora-pdf-content">
    <?php print $islandora_content; ?>
  </div>
  
  <?php if (isset($islandora_download_link)): ?>
  <div class="islandora-download-link">
    <?php print $islandora_download_link; ?>
  </div>
  <?php endif; ?>
  
<?php endif; ?>
</div>
```

####After
**/var/www/your_Drupal_Site/sites/themes/your_Theme/islandora-pdf.tpl.php**
```html
<!------- SNIP ---------->
<div class="islandora-pdf-content-wrapper clearfix">
<?php if (isset($islandora_content)): ?>
  
  <div class="islandora-pdf-content">
    <?php print $islandora_content; ?>
  </div>
  
  <?php if (isset($islandora_download_link)): ?>
  <div class="islandora-download-link">
    <?php print $islandora_download_link; ?>
  </div>
  <?php endif; ?>
  
<?php endif; ?>
</div>

<!-- usage stats -->
<?php if (module_exists('islandora_usage_stats')): ?>
<div class="islandora-usage-stats">
  <!-- we have added some bootstrap specific css classes (badge) to our view counts, include if desired -->
  <p>
    Viewed: <span class="badge"><?php print $times_viewed ?></span> Last viewed: <?php print $time_last_viewed==NULL ? 'Never' : date('g:ia, M d, Y', $time_last_viewed); ?>
  </p>
  <p>
    Downloaded: <span class="badge"><?php print $times_downloaded ?></span> Last downloaded: <?php print $time_last_downloaded==NULL ? 'Never' : date('g:ia, M d, Y', $time_last_downloaded); ?>
  </p>
</div>
<?php endif; ?>
<!-- end usage stats -->
```


