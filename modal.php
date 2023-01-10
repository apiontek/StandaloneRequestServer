<?php
include('global.inc');

// Reduce multiple spaces to single spaces, and trim start & end whitespace
// $input_query = trim(preg_replace('!\s+!', ' ', $_POST['q']));

// echo $input_query;

echo '<div id="modal">
  <div class="modal-underlay" onClick="removeModal()"></div>
  <div class="modal-content">
    <h2>Modal Dialog</h2>
    This is the modal content.
    You can put anything here, like text, or a form, or an image.
    <br>
    <br>
    <button onClick="removeModal()">Close</button>
  </div>
</div>';


die();

?>