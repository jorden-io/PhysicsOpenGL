let even = 0;
document.getElementById("understand").addEventListener("click", (e) => {
  even += 1;
});
const radius = 15;
const vector = {
  x: 0,
  y: 0,
};
let move_x = 2;
let move_y = 2;
const canvas = document.getElementById("myCanvas");
const ctx = canvas.getContext("2d");
document.getElementById("exit").addEventListener("click", (e) => {
  if (even > 3) {
    setInterval(() => {
      ctx.clearRect(0, 0, 400, 300);
      if (velocity.x > canvas.width - radius || velocity.x < radius) {
        move_x = move_x * -1;
      }
      if (velocity.y > canvas.height - radius || velocity.y < radius) {
        move_y = move_y * -1;
      }
      velocity.x += move_x;
      velocity.y += move_y;
      ctx.beginPath();
      ctx.fillStyle = "green";
      ctx.arc(velocity.x, velocity.y, radius, 0, Math.PI * 2);
      ctx.fill();
      ctx.closePath();
    }, 10);
  }
});
if (even % 2 === 1) {
  document.getElementById("med-content").style.filter = "blur(0px)";
  document.getElementById("pop-up-outer").style.display = "none";
}
