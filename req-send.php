<?php
include('global.inc');

$songid = $_POST['songid'];
$singer = $_POST['singer'];

if ($singer == '') {
  echo "<input type=\"hidden\" name=\"songid\" value=\"$songid\">
    <label>Who will sing it?</label>
    <input class=\"error\" type=\"text\" name=\"singer\" autocomplete=\"off\" autofocus placeholder=\"Enter your name or nickname\">
    <p class=\"error\">Sorry, you must input a singer name.  Please go back and try again.</p>
    <div class=\"req-modal-buttons\">
      <button type=\"button\" onClick=\"removeReqModal()\" class=\"close\">Close</button>
      <input type=\"submit\" value=\"Send Request\">
    </div>";
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
  <button type=\"button\" onClick=\"removeReqModal()\" class=\"close\">Close</button>
  </div>";

die();

?>