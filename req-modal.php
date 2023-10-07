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


echo "<div
    id=\"req-modal\"
    hx-on:remove-req-modal=\"
      this.addEventListener('animationend', function () { this.remove(); });
      htmx.addClass(this, 'closing');
    \"
    >
  <div
    class=\"req-modal-underlay\"
    hx-on:click=\"htmx.trigger('#req-modal', 'remove-req-modal');\"
    ></div>
  <div id=\"req-song-content\" class=\"req-modal-content\">
    <h2>Requesting Song</h2>
    <p class=\"song\">$artist - $title</p>
    <form
      id=\"req-modal-form\"
      hx-post=\"req-send.php\"
      >";
reqFormContent($songid);
echo "</form>
  </div>
</div>";


die();

?>