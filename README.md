# 🔎FPGA 보드를 활용한, 절대음감 테스트 안드로이드 애플리케이션 - perfectPitch

### 개요

* FPGA보드의 디바이스 드라이버를 활용하여 안드로이드 애플리케이션 제작
* 절대음감 테스트하기, 보드의 버튼을 피아노 연주처럼 활용 가능

### 개발 목적

* 간단한 피아노 연주도 가능하고, 테스트도 가능한 재미있는 애플리케이션 제작

### 개발 기간

* 2018.09~2018.11

### 설계 환경

* 개발 환경 : Linux(Ubuntu)/Eclipse(Android)
* 타겟 보드 : (주)휴인스 Exynos-5250 
  * ARM Cortex A-15 Processor

### 구현 기능

* **절대음감 테스트 기능**

  * 시작 버튼을 누르면 음 3개 출력
  * 스레드가 시작되면서, 10초의 시간이 주어짐(0이 될 때까지 시간이 줄어듬)
  * 출력된 3개의 음을 순서대로 입력해야 함
    * 스테이지 클리어 시 - 다음 버튼을 누르면 다음 스테이지로 이동
    * 스테이지 실패 시(시간 초과) - 버저가 울리면서 Game Over
    * 총 8개의 스테이지 통과 시 - TEXT LCD에 "축하축하" 표시

* **피아노 기능**

  * PUSH SWITCH 를 이용하여 간단한 피아노 연주 가능, 버튼을 누르면 순서대로 C~높은C까지 출력

* **사용된 디바이스 드라이버**

  ![DeviceDriver](/res/그림1.png)

  * **Fnd** - 행운의 7777 표시
  * **LED** - 랜덤으로 반짝반짝 불빛 점등
  * **TEXT LCD** - 스테이지 상태에 따른 문구 표시
  * **DOT MATRIX** - 현재 스테이지 표시
  * **PUSH SWITCH** -피아노 버튼
  * **Step Moter** - 스테이지 클리어 시 회전
  * **Buzzer** - 시간 초과 시 소리 재생

### 파일

* jni.c
  * 사용하는 7개의 디바이스 드라이버에 대한 소스
* PushSwitchActivity.java
  * 안드로이드 프로그램 메인 자바 소스

### 데모 영상

* 성공 영상

  <iframe width="560" height="315" src="https://www.youtube.com/embed/fl-sXy8q4RM" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

* 실패 영상

  <iframe width="560" height="315" src="https://www.youtube.com/embed/h-gax7JYiXc" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

  

