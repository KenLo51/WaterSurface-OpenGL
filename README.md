# ComputerGraph-WaterSurface

1. OpenGL FBO(Frame Buffer Object)製作水面反射(Reflection map)、折射(Refraction map)。  
2. 依據當下Camera位置，於相同位置但限制Refraction View確保可觀察水面正下方，並繪製Refraction Map。  
3. Reflection View使用當下Camera位置與水面鏡射位置繪製Reflection Map。  
4. 依據陽光方向繪製Depth Map用以製作陰影，繪製Depth Map時只會至物體背面(glCullFace(GL_BACK);)。  
5. 模擬2D波動參考["Wave equation - Wikipedia"](https://en.wikipedia.org/wiki/Wave_equation)，每偵更新2次微分數值計算ODE。因此若時間間隔(ODE中各項係數設定)錯誤，可能使其結果發散無法正確模擬。於/source code/pythonWaveSimulation/中為ODE公式驗證用以設定ODE中系數。

#### Libraries
1. [Glad](https://glad.dav1d.de/)<br>
2. [GLFW](https://www.glfw.org/)<br>
2. [OpenGL 3.3](https://www.opengl.org/)<br>
3. [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)<br>
4. [Dear ImGui](https://github.com/ocornut/imgui)<br>

 
 #### Materials
 1. [Floor Tiles 09](https://polyhaven.com/a/floor_tiles_09)
 
#### 功能說明
<img src="https://i.imgur.com/af1vum4.png" width="697" height="672" />


Title : 顯示目前偵率。<br>
User Manual : 基本操作提示。<br>
>  (1) ESC 關閉程式。<br>
>  (2) F1 隱藏GUI。<br>
>  (3) WASD 控制視角水平移動。<br>
>  (4) - = 調整移動速度。<br>
>  (5) 滑鼠右鍵拖曳旋轉視角。<br>
>  (6) 滑鼠左鍵可在模擬時產生水波。<br>

Water : 調整水面參數。<br>
Sun light : 調整光源參數。<br>


#### 運行結果
|<img src="https://i.imgur.com/mlWGNoW.png" width="400" height="233" />|<img src="https://i.imgur.com/oBfLAS8.png" width="400" height="233" />|
| :-: | :-: |
| 以 Sin 函數製作水波 | 水波模擬 |
|<img src="https://i.imgur.com/2RsL3Ey.png" width="400" height="233" />|<img src="https://i.imgur.com/nIYqwnY.png" width="400" height="233" />|  
| 以 Height Map 製作水波 | 水中焦散效果 |

<img src="https://i.imgur.com/UlLyZ3R.gif" width="200" height="200" />  
Python中模擬2維波動結果  

#### 待做
1. 使用PBO(Pixel Buffer Object)減少模擬結果傳遞至GPU時間。
2. 在使用更多FBO直接於GPU模擬水面波動。  
3. Height map改為使用3D Texture。  
4. perlin noise製作水面波浪。  
