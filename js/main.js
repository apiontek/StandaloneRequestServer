// new song request modal needs help closing
function removeReqModal() {
  var reqModal = document.getElementById('req-modal');
  if (reqModal) {
    reqModal.addEventListener('animationend', () => {
      reqModal.remove();
    });
    reqModal.classList.add('closing');
  }
}
// Esc key can also close the song request modal
document.addEventListener('keydown', (event) => {
  if (event.key === 'Escape') { removeReqModal(); }
})

// Enable/Disable request modal button on input entry
function enableDisableReqBtn(inputEl) {
  //Reference the Button.
  var btnSubmit = document.getElementById("send-song-request");

  // Verify the input value.
  var inputVal = inputEl.value.trim();
  if (inputVal != "" && inputVal.length > 1) {
      // Enable the form button when input has value.
      btnSubmit.disabled = false;
  } else {
      // Disable the form button when input is empty.
      btnSubmit.disabled = true;
  }
};