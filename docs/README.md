# IRC 채팅 서버

이 프로젝트는 C++을 사용하여 구현된 IRC(Internet Relay Chat) 프로토콜 기반 채팅 서버입니다. </br>

<details>
  <summary>프로젝트 요구사항</summary>
  
  1. **기본 요구사항**:
      - 서버는 여러 클라이언트를 동시에 처리할 수 있어야 하며, 절대 멈추지 않아야 합니다.
      - Fork를 사용하지 않고, 모든 I/O 작업은 non-blocking이어야 합니다.
      - Poll() (또는 유사한 것 ex: kqueue) 하나만을 사용하여 모든 작업 (읽기, 쓰기, 듣기 등)을 처리해야 합니다.
      - 서버는 TCP/IP (v4 또는 v6)를 통해 클라이언트와 통신해야 합니다.
      - 클라이언트가 서버에 연결하기 위해서는 포트 번호와 연결 비밀번호가 필요합니다.
  2. **클라이언트와 서버 간의 커뮤니케이션**:
      - 클라이언트는 서버에 연결하고 채널에 가입할 수 있어야 합니다.
      - 서버는 클라이언트가 보낸 메시지를 해당 채널의 모든 다른 클라이언트에게 전달해야 합니다.
      - 사용자 인증, 닉네임 설정, 유저네임 설정, 채널 참여, 개인 메시지 송수신 등의 기능을 구현해야 합니다.
  3. **채널 운영자 및 사용자 권한**:
      - 채널 운영자와 일반 사용자를 구분해야 합니다.
      - 채널 운영자 전용 명령어 (예: KICK, INVITE, TOPIC, MODE 등)를 구현해야 합니다.
  4. **추가 요구사항**:
      - 코드는 C++ 98 표준을 준수해야 하며, 필요한 경우 **`std=c++98`** 플래그를 추가하여 컴파일할 수 있어야 합니다.
      - Makefile을 사용하여 소스 파일을 컴파일해야 하며, 프로그램은 어떠한 상황에서도 충돌하거나 예상치 못하게 종료되어서는 안됩니다.
  
    이 정보를 바탕으로, IRC 서버를 설계할 때 각 기능별로 분리하여 구현하는 것이 좋습니다. 예를 들어, 네트워크 연결 처리, 사용자 인증, 메시지 전달 및 처리, 채널 관리 등을 별도의 모듈로 구성할 수 있습니다. 
    
    코드는 간결하고 이해하기 쉬워야 하며, 다양한 에러 상황에서도 안정적으로 작동해야 합니다.
  
</details>

 </br></br></br>
 
## 과제 세부 내용

과제에 대한 세부 정보는 다음 블로그를 통해 확인할 수 있습니다 </br>
[블로그 링크](https://blog.naver.com/PostList.naver?blogId=bluedog129&from=postList&categoryNo=60)


</br></br>

## 실행 방법 및 실행 예시

### 실행 방법

채팅 서버를 실행하기 위해서는 다음의 단계를 따라야 합니다:
</br>
### 우선 채팅서버를 실행시킵니다.
1. 소스 코드를 컴파일합니다. `make`
2. 컴파일된 실행 파일을 실행합니다. `./ircserv`
<details>
  <summary>서버실행 영상</summary>
  

https://github.com/bluedog129/IRC/assets/50707297/30dfbdcc-93de-4241-b818-635476f31466


</details>


### irssi 프로그램을 실행하여 클라이언트로서 접속을 시도합니다.
1. 새로운 터미널에 irssi 명령어를 실행합니다.
2. "/connect -nocap 127.0.0.1 6667 12345678 [유저이름]" 으로 채팅프로그램에 접속을 시도합니다.
<details>
  <summary>irssi 실행 영상</summary>
  

https://github.com/bluedog129/IRC/assets/50707297/c67810b8-21b3-4e5f-ab81-3045b8466b72


</details>


</br></br>

### 실행 예시
실행되는 모습은 아래 영상을 통해 확인할 수 있습니다:

<details>
  <summary>join 명령어</summary>
  

https://github.com/bluedog129/IRC/assets/50707297/bfe47e6b-3095-4394-8d88-f9e108579cf1


</details>

<details>
  <summary>part 명령어</summary>
  

https://github.com/bluedog129/IRC/assets/50707297/372bd868-4e4c-42a8-aa78-1dfbe6b4dfc8


</details>

<details>
  <summary>mode +o 명령어</summary>
  - /mode: IRC에서 채널이나 사용자의 모드를 설정하거나 조회하는 데 사용되는 명령어입니다. </br>
  - +o: 이는 "operator flag"를 설정하는 것으로, 대상 사용자에게 오퍼레이터 권한을 부여합니다. 오퍼레이터는 채널에서 사용자를 추방하거나, 채널 모드를 변경하는 등의 관리 작업을 수행할 수 있습니다. </br>
  - 사용자이름: 오퍼레이터 권한을 부여하고자 하는 사용자의 닉네임입니다. </br>
  

https://github.com/bluedog129/IRC/assets/50707297/5feadb1f-5f43-4740-a903-6fd8b567a4b6


</details>

</details>

<details>
  <summary>mode +i, invite 명령어</summary>
  - /mode +i 명령어는 채널에 "초대 전용(invite-only)" 모드를 설정합니다. 이 모드가 설정되면, 채널에 들어올 수 있는 사람들을 제한하여 오직 초대받은 사용자만이 채널에 접근할 수 있게 됩니다. 이는 특정 주제에 대해 논의하거나, 특정 그룹의 멤버만을 위한 공간을 만들고자 할 때 유용합니다.

  - /invite 명령어는 특정 사용자를 현재의 채널로 초대하는 데 사용됩니다. 채널이 초대 전용 모드일 때 더욱 유용하며, 이 경우 초대받은 사용자만이 채널에 입장할 수 있습니다.
  

https://github.com/bluedog129/IRC/assets/50707297/5016fc36-fbdd-4daf-9dae-8ee176567b80


</details>

<details>
  - /mode #채널이름 +k 비밀번호 </br>
  - mode +k는 해당 채널에 비밀번호를 적용하는 명령어
  <summary>mode +k 명령어</summary>
  

https://github.com/bluedog129/IRC/assets/50707297/a2585918-d7fa-4017-a4ad-8f392eeb0289


</details>

