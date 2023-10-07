<?php
include('global.inc');

$songid = $_POST['songid'];
$singer = $_POST['singer'];

if ($singer == '') {
  reqFormContent($songid);
  die();
}

$entries = null;
$wherestring = null;
$artist = '';
$title = '';
$sql = "SELECT artist,title FROM songdb WHERE song_id = $songid";
foreach ($db->query($sql) as $row) {
  $artist = $row['artist'];
  $title = $row['title'];
}
$stmt = $db->prepare("INSERT INTO requests (singer,artist,title) VALUES(:singer, :artist, :title)");
$stmt->execute(array(":singer" => $singer, ":artist" => $artist, ":title" => $title));
newSerial();

echo "<p>Request sent for $singer</p>
  <div class=\"req-modal-buttons\">
  <button
    type=\"button\"
    class=\"close\"
    hx-on:click=\"htmx.trigger('#req-modal', 'remove-req-modal');\"
    >Close</button>
  </div>";

die();

?>