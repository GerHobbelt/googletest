// Pipeline definition for the build steps of AR.
//
// Copyright: 2017 Ditto Technologies. All Rights Reserved.
// Author: Frankie Li, Daran He, John Inacay

// TODO: Need to migrate to a standardize Debian package deployment script.

properties([[
  $class: 'BuildDiscarderProperty',
  strategy: [
    $class: 'LogRotator',
    artifactDaysToKeepStr: '',
    artifactNumToKeepStr: '',
    daysToKeepStr: '',
    numToKeepStr: '5'
  ]
]]);

@Library('jenkins-shared-library@olegs-test') _

def BUILD_CONFIGS = [
    'ubuntu_16_04' : [
        'docker_name' : 'ubuntu16-build-env',
        'docker_file' : 'Dockerfile.xenial',
        'repo'        : '3rdparty-16.04',
        'staging_repo': '3rdparty-16.04-staging',
        'dist'        : 'xenial',
    ],
    'ubuntu_14_04' : [
        'docker_name' : 'ubuntu14-build-env',
        'docker_file' : 'Dockerfile.trusty',
        'repo'        : '3rdparty-14.04',
        'staging_repo': '3rdparty-14.04-staging',
        'dist'        : 'trusty',
    ]
]

def S3_PACKAGE_CREDS = "package-uploads"

node('build && docker') {
  BUILD_CONFIGS.each { target, build_config ->
    def docker_name = build_config.docker_name
    def docker_file = build_config.docker_file
    def staging_repo = build_config.staging_repo
    def repo = build_config.staging_repo
    def dist = build_config.dist

    dir(target) {
      stage("Checkout ${target}") {
        git_info = ditto_git.checkoutRepo()
      }

      stage("Build and Publish to Test Repo ${target}") {
        ditto_docker.buildDockerImage(docker_name, docker_file)
        ditto_docker.deleteDockerOutdated()

        version_number = ditto_deb.getDittoVersion()
        revision = ditto_deb.getDevRevision(git_info.commit)

        ditto_deb.buildSource(docker_name)
        ditto_deb.buildDebianPackage(docker_name, version_number, revision)
        archiveArtifacts(artifacts: 'build/*.deb')
        ditto_deb.publishDebToS3(staging_repo, dist, S3_PACKAGE_CREDS)
      }

      stage("Install and Test from Test Repo ${target}") {
      }
    }
  }
}

// Master Node.
stage("Tag and deploy?") {
  deploy_mode = "SKIP"
  // if (git_info.is_release) {
    deploy_mode = input(
      message: "User input required",
      parameters: [
        choice(
          name: "Deploy \"${version_number}\" at hash " +
                " \"${git_info.commit}\"?",
          choices: [ "SKIP", "RC", "RELEASE" ].join("\n"))])
  // }
}

node('build && docker') {
  BUILD_CONFIGS.each { target, build_config ->
    docker_name = build_config.docker_name
    staging_repo = build_config.staging_repo
    repo = build_config.staging_repo
    dist = build_config.dist

    if (deploy_mode != "SKIP") {
      stage("Build and Publish to Test Repo ${target}") {
        if (deploy_mode == "RC") {
          new_rc_number = ditto_git.calcRcNumber(version_number)
          tag = ditto_git.getRcTag(version_number, new_rc_number)
          revision = ditto_deb.getRcRevision(rc_number)
          publish_repo = staging_repo
        } else if (deploy_mode == "RELEASE") {
          tag = ditto_git.getReleaseTag(version_number)
          revision = ditto_deb.getReleaseRevision()
          publish_repo = repo
        }

        // Build and publish.
        ditto_deb.buildDebianPackage(docker_name, version_number, revision)
        archiveArtifacts(artifacts: 'build/*.deb')
        ditto_deb.publishDebToS3(publish_repo, dist, S3_PACKAGE_CREDS)

        // Push tags.
        ditto_git.pushTag(rc_tag)
      }
    }

    stage("Clean up ${target}") {
      deleteDir()
    }
  }
}
