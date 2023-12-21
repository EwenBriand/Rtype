<template>
    <div class="desktop">
      <div class="overlap">
        <div class="overlap-group">
          <div class="button-rectangle" />
          <div class="div" />
          <div class="red-circle" />
          <div class="image-red-shadow" />
          <div class="puple-circle" />
          <div class="window" />
          <div class="title">Recents</div>
          <div class="text-wrapper">Welcome to SlashR Gaby</div>
          <div class="button-open">
            <div class="div-wrapper">
              <div class="text-wrapper-2">Open</div>
            </div>
          </div>
          <div class="button-create">
            <div class="overlap-2">
              <div class="text-wrapper-3">Create new</div>
            </div>
          </div>
          <div class="side-image">
            <div class="overlap-3">
              <img class="spaceship-image" alt="Spaceship image" src="../assets/preview-image.png" />
              <div class="text-wrapper-4">Edit your games</div>
              <p class="p">Create and edit your games as wanted</p>
            </div>
          </div>
          <div class="projects-section">
            <img class="line" alt="Line" src="../assets/separator.svg" />
            <img class="img" alt="Line" src="../assets/separator.svg" />
            <img class="line-2" alt="Line" src="../assets/separator.svg" />
            <img class="line-3" alt="Line" src="../assets/separator.svg" />
            <img class="line-4" alt="Line" src="../assets/separator.svg" />
            <img class="line-5" alt="Line" src="../assets/separator.svg" />
            <div class="project">BelowTale - Remastered</div>
            <div class="project-2">League of Superstar</div>
            <div class="project-3">Castlenite</div>
            <div class="project-4">Super blast bros.</div>
            <div class="project-5">Asiatic sword zero</div>
          </div>
        </div>
        <div class="transparent-circle" />
        <div class="transparent-circle-2" />
      </div>
      <div class="big-red-circle" />
    </div>
</template>

<script>
  export default {
    name: "slashrLauncher",
  };
</script>

<style>
  .desktop {
    background-color: #121212;
    height: 800px;
    overflow: hidden;
    position: relative;
    width: 1280px;
  }

  .desktop .overlap {
    height: 839px;
    left: 161px;
    position: absolute;
    top: -47px;
    width: 1096px;
  }

  .desktop .overlap-group {
    height: 795px;
    left: 0;
    position: absolute;
    top: 0;
    width: 1096px;
  }

  .desktop .button-rectangle {
    background-color: #ad2929;
    border-radius: 25px;
    height: 37px;
    left: 317px;
    position: absolute;
    top: 592px;
    width: 140px;
  }

  .desktop .div {
    background-color: #ad2929;
    border-radius: 25px;
    height: 37px;
    left: 140px;
    position: absolute;
    top: 592px;
    width: 139px;
  }

  .desktop .red-circle {
    background: linear-gradient(180deg, rgb(238, 66.44, 66.44) 0%, rgba(225, 135, 135, 0.74) 100%);
    border-radius: 70px;
    height: 140px;
    left: 0;
    position: absolute;
    top: 655px;
    width: 140px;
  }

  .desktop .image-red-shadow {
    background: linear-gradient(180deg, rgb(173.19, 41.85, 41.85) 0%, rgb(206, 47.6, 26) 100%);
    border-radius: 155.5px/156.5px;
    height: 313px;
    left: 611px;
    position: absolute;
    top: 297px;
    width: 311px;
  }

  .desktop .puple-circle {
    background: linear-gradient(180deg, rgba(173, 72, 188, 0.58) 0%, rgb(206.13, 60.98, 156.78) 100%);
    border-radius: 127px/128px;
    height: 256px;
    left: 842px;
    position: absolute;
    top: 0;
    width: 254px;
  }

  .desktop .window {
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    background-color: #27272780;
    border-radius: 30px;
    box-shadow: inset 0px 4px 20px #ffffff33;
    height: 578px;
    left: 41px;
    position: absolute;
    top: 161px;
    width: 928px;
  }

  .desktop .title {
    color: #ffffff;
    font-family: "Roboto-Medium", Helvetica;
    font-size: 24px;
    font-weight: 500;
    left: 252px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 324px;
    width: 95px;
  }

  .desktop .text-wrapper {
    color: #ffffff;
    font-family: "Roboto-Bold", Helvetica;
    font-size: 32px;
    font-weight: 700;
    left: 82px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 207px;
    width: 375px;
  }

  .desktop .button-open {
    height: 37px;
    left: 317px;
    position: absolute;
    top: 592px;
    width: 142px;
  }

  .desktop .div-wrapper {
    background-color: #ad2929;
    border-radius: 25px;
    height: 37px;
    position: relative;
    width: 140px;
  }

  .desktop .text-wrapper-2 {
    color: #ffffff;
    font-family: "Roboto-Medium", Helvetica;
    font-size: 13px;
    font-weight: 500;
    left: 52px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 10px;
    width: 35px;
  }

  .desktop .button-create {
    height: 37px;
    left: 140px;
    position: absolute;
    top: 592px;
    width: 141px;
  }

  .desktop .overlap-2 {
    background-color: #ad2929;
    border-radius: 25px;
    height: 37px;
    position: relative;
    width: 139px;
  }

  .desktop .text-wrapper-3 {
    color: #ffffff;
    font-family: "Roboto-Medium", Helvetica;
    font-size: 13px;
    font-weight: 500;
    left: 34px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 10px;
    width: 70px;
  }

  .desktop .side-image {
    height: 381px;
    left: 628px;
    position: absolute;
    top: 297px;
    width: 281px;
  }

  .desktop .overlap-3 {
    background-color: #363636;
    border-radius: 15px;
    height: 381px;
    position: relative;
    width: 277px;
  }

  .desktop .spaceship-image {
    height: 295px;
    left: 0;
    object-fit: cover;
    position: absolute;
    top: 0;
    width: 277px;
  }

  .desktop .text-wrapper-4 {
    color: #ffffff;
    font-family: "Roboto-SemiBold", Helvetica;
    font-size: 16px;
    font-weight: 600;
    left: 19px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 311px;
    width: 134px;
  }

  .desktop .p {
    color: #ffffff;
    font-family: "Roboto-Light", Helvetica;
    font-size: 13px;
    font-weight: 300;
    left: 21px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 344px;
    width: 235px;
  }

  .desktop .projects-section {
    height: 141px;
    left: 174px;
    position: absolute;
    top: 392px;
    width: 261px;
  }

  .desktop .line {
    height: 2px;
    left: 0;
    position: absolute;
    top: 26px;
    width: 251px;
  }

  .desktop .img {
    height: 2px;
    left: 0;
    position: absolute;
    top: -2px;
    width: 251px;
  }

  .desktop .line-2 {
    height: 2px;
    left: 0;
    position: absolute;
    top: 55px;
    width: 251px;
  }

  .desktop .line-3 {
    height: 2px;
    left: 0;
    position: absolute;
    top: 82px;
    width: 251px;
  }

  .desktop .line-4 {
    height: 2px;
    left: 0;
    position: absolute;
    top: 110px;
    width: 251px;
  }

  .desktop .line-5 {
    height: 2px;
    left: 0;
    position: absolute;
    top: 139px;
    width: 251px;
  }

  .desktop .project {
    color: #ffffff;
    font-family: "Roboto-Light", Helvetica;
    font-size: 11px;
    font-weight: 300;
    left: 23px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 8px;
    white-space: nowrap;
    width: 118px;
  }

  .desktop .project-2 {
    color: #ffffff;
    font-family: "Roboto-Light", Helvetica;
    font-size: 11px;
    font-weight: 300;
    left: 23px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 36px;
    white-space: nowrap;
    width: 99px;
  }

  .desktop .project-3 {
    color: #ffffff;
    font-family: "Roboto-Light", Helvetica;
    font-size: 11px;
    font-weight: 300;
    left: 23px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 63px;
    white-space: nowrap;
    width: 49px;
  }

  .desktop .project-4 {
    color: #ffffff;
    font-family: "Roboto-Light", Helvetica;
    font-size: 11px;
    font-weight: 300;
    left: 23px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 91px;
    width: 82px;
  }

  .desktop .project-5 {
    color: #ffffff;
    font-family: "Roboto-Light", Helvetica;
    font-size: 11px;
    font-weight: 300;
    left: 23px;
    letter-spacing: 0;
    line-height: normal;
    position: absolute;
    top: 120px;
    white-space: nowrap;
    width: 90px;
  }

  .desktop .transparent-circle {
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    background-color: #1212120d;
    border-radius: 43.5px/44.5px;
    box-shadow: inset 0px 4px 12px #ffffff66, inset 0px -40px 40px #00000033, inset 0px -4px 4px #ffffff40,
      inset 0px 20px 40px #fffbfb40, inset 0px 1px 2px #ffffffcc;
    height: 89px;
    left: 714px;
    position: absolute;
    top: 750px;
    width: 87px;
  }

  .desktop .transparent-circle-2 {
    -webkit-backdrop-filter: blur(40px) brightness(100%);
    backdrop-filter: blur(40px) brightness(100%);
    background-color: #cc1e1e0d;
    border-radius: 44px/44.5px;
    box-shadow: inset 0px 4px 12px #ffffff66, inset 0px -40px 40px #00000033, inset 0px -4px 4px #ffffff40,
      inset 0px 20px 40px #fffbfb40, inset 0px 1px 2px #ffffffcc;
    height: 89px;
    left: 287px;
    position: absolute;
    top: 47px;
    transform: rotate(-180deg);
    width: 88px;
  }

  .desktop .big-red-circle {
    background: linear-gradient(180deg, rgba(225, 135, 135, 0.74) 0%, rgb(238, 66.44, 66.44) 100%);
    border-radius: 115.5px/108.5px;
    height: 217px;
    left: -70px;
    position: absolute;
    top: -74px;
    width: 231px;
  }
</style>
