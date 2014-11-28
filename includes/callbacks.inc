<?php
/**
 * @file
 * Menu alter hook callbacks.
 */

/**
 * Menu alter replacement callback for islandora/search.
 *
 * @param string $term
 *   The search term to track.
 */
function islandora_usage_stats_track_search_results($term = NULL) {
  module_load_include('module', 'islandora_solr', "islandora_solr");
  if(!empty($term)) {
    IslandoraUsageStats::trackSearchTerm($term);
  }
  $output = islandora_solr($term);
  return $output;
}

/**
 * Menu alter replacement callback for ds downloads.
 *
 * @param AbstractDatastream $datastream
 *   The datastream being downloaded.
 */
function islandora_usage_stats_track_downloads(AbstractDatastream $datastream) {
  module_load_include('inc', 'islandora', "includes/datastream");
  $ds_name = $datastream->id;
  $parent_id = $datastream->parent->id;
  $track = new IslandoraUsageStats($parent_id);
  $track->addTopDownload($ds_name);
  islandora_download_datastream($datastream);
}
