<h3>preview</h3>
<img src="./screenshots/terrain.jpeg" title="exe" alt="terrain" style="width: 500px;">
<img width="500" alt="terrain" src="https://user-images.githubusercontent.com/101066744/229156153-b8b74174-1771-49bd-a63f-14e7a52154f0.png">
<h4>compile with:</h4>
<h5>clang++ -Wall -std=c++17 -I ./dependencies/include -L ./dependencies/library -fcolor-diagnostics -fansi-escape-codes -g  main.cpp glad.c ./dependencies/library/libglfw.3.3.dylib -o ./out -framework OpenGL  -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated</h5>
<hr>
<h3>CONTROLS</h3>
<h5>all lower case</h5>
<ul>
  <li>WASD: Movment</li>
  <li>U: Speeds up movement when pressed</li>
  <li>R: Rotates all items</li>
  
  <li>I: Grows items amounts</li>
  <li>L: Shrinks items amount</li>
  
  <li>P: Increases magnitude of hills</li>
  <li>L: Decreases magnitude of hills</li>
  
  <li>M: Increases total hills amount</li>
  <li>N: Decreases total hills amount</li>
  
  <li>O: Decreases item amount by fixed number (dont use, experimental.)</li>
</ul>
