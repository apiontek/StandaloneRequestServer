<?php 
include('global.inc');
siteheader('Search Results');
navbar("index.php");

// Reduce multiple spaces to single spaces, and trim start & end whitespace
$input_query = trim(preg_replace('!\s+!', ' ', $_GET['q']));

// Validate that query is not just whitespace, and is 3 or more characters
if (ctype_space($input_query) || strlen($input_query) < 3) {
  echo "<p>You must enter at least 3 characters as a search query.</p>";
  die();
}

$terms = explode(' ',$input_query);
$no = count($terms);
$wherestring = '';
if ($no == 1) {
	$wherestring = "WHERE (combined LIKE \"%" . $terms[0] . "%\")";
} elseif ($no >= 2) {
  foreach ($terms as $i => $term) {
    if ($i == 0) {
      $wherestring .= "WHERE ((combined LIKE \"%" . $term . "%\")";
    }
    if (($i > 0) && ($i < $no - 1)) {
      $wherestring .= " AND (combined LIKE \"%" . $term . "%\")";
    }
    if ($i == $no - 1) {
      $wherestring .= " AND (combined LIKE \"%" . $term . "%\") AND(artist != 'DELETED'))";
    }
  }
} else {
	echo "<li>You must enter at least one search term</li>";
	die();
}

$entries = null;
$res = array();
    $sql = "SELECT song_id,artist,title,combined FROM songdb $wherestring ORDER BY UPPER(artist), UPPER(title)";
    foreach ($db->query($sql) as $row)
        {
	if ((stripos($row['combined'],'wvocal') === false) && (stripos($row['combined'],'w-vocal') === false) && (stripos($row['combined'],'vocals') === false)) {
		$res[$row['song_id']] = $row['artist'] . " - " . $row['title'];
	}
        }
    $db = null;

$unique = array_unique($res);

foreach ($unique as $key => $val) {
	$entries[] = "<button class=\"result\" onclick=\"submitreq(${key})\">" . $val . "</button>";
}

echo "<p><strong>Search Results for \"$input_query\"</strong>";

if (count($unique) > 0) {
	echo '<br/>Tap a song to request it.</p>';
	foreach ($entries as $song) {
		echo $song;
	}
} else {
	echo "</p><p>Sorry, no match found.</p>";
}

sitefooter();
?> 
