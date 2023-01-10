<?php
include('global.inc');

$songid = $_POST['songid'];

$artist = '';
$title = '';
$sql = "SELECT artist,title FROM songdb WHERE song_id = $songid";
foreach ($db->query($sql) as $row) {
        $artist = $row['artist'];
        $title = $row['title'];
}
$db = null;

echo "<div id=\"req-modal\">
  <div class=\"req-modal-underlay\" onClick=\"removeReqModal()\"></div>
  <div id=\"req-song-content\" class=\"req-modal-content\">
    <h2>Requesting Song</h2>
    <p class=\"song\">$artist - $title</p>
    <form hx-post=\"req-send.php\">
      <input type=\"hidden\" name=\"songid\" value=\"$songid\">
      <label>Who will sing it?</label>
      <input type=\"text\" name=\"singer\"
        autocomplete=\"off\" autofocus
        placeholder=\"Enter your name or nickname\"
        onKeyUp=\"enableDisableReqBtn(this)\">
      <div class=\"req-modal-buttons\">
        <button type=\"button\" onClick=\"removeReqModal()\" class=\"close\">Close</button>
        <input type=\"submit\" id=\"send-song-request\" value=\"Send Request\" disabled>
      </div>
    </form>
  </div>
</div>";


die();

?>