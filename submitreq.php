<?php
include('global.inc');
siteheader('Submit Request');
$referer = $_SERVER['HTTP_REFERER'];
if (strpos($referer,'submitreq-run.php') !== false)
{
  navbar("index.php");
} else {
  navbar($referer);
}
$songid = $_GET['id'];

$artist = '';
$title = '';
$sql = "SELECT artist,title FROM songdb WHERE song_id = $songid";
foreach ($db->query($sql) as $row) {
        $artist = $row['artist'];
        $title = $row['title'];
}
$db = null;
echo "<p><strong>Requesting song:</strong><br/><span class=\"song\">$artist - $title</span></p>";
echo "<form method=\"get\" action=\"submitreq-run.php\">
  <input type=\"hidden\" name=\"songid\" value=\"$songid\">
  <label>Enter your name or nickname</label>
  <input type=\"text\" name=\"singer\" autocomplete=\"off\" autofocus>
  <input type=\"submit\" value=\"Submit Request\">
  </form>";
echo "<details open>
    <summary>Instructions</summary>
    <p>If you have a common first name, please also enter your last initial or last name. Doing so will help eliminate confusion and reduce the risk of your turn getting skipped.</p>
  </details>";
?>
