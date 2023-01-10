<?php
include('global.inc');

// Reduce multiple spaces to single spaces, and trim start & end whitespace
$input_query = trim(preg_replace('!\s+!', ' ', $_POST['q']));

// Validate that query is not just whitespace, and is 3 or more characters
if (ctype_space($input_query) || strlen($input_query) < 3) {
  helpHints();
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
  helpHints();
  die();
}

$accepting = getAccepting();
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
  if ($accepting) {
    $entries[] = "<button class=\"result song\" hx-post=\"/req-modal.php\" hx-target=\"body\" hx-swap=\"beforeend\" hx-vals='{\"songid\":\"${key}\"}'>" . $val . "</button>";
  } else {
    $entries[] = "<button class=\"result song\">" . $val . "</button>";
  }
}

$count = 0;
if ($entries) $count = count($entries);
$results_str = 'results';
if ($count === 1) $results_str = 'result';
echo "<p><strong>$count search $results_str for \"$input_query\"</strong>";

if (count($unique) > 0) {
  if ($accepting) {
    echo '<br/>Tap a song to request it.</p><div>';
  } else {
    echo '</p><div class="not-accepting">';
  }
  foreach ($entries as $song) {
    echo $song;
  }
  echo '</div>';
} else {
  echo "</p><p>Sorry, no match found.</p>";
}

?>
