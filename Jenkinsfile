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

BUILD_CONFIGS.each { target, build_config ->
  node('build && docker') {
    stage("Checkout ${target}") {
      git_info = ditto_git.checkoutRepo()
    }

    stage("Prepare Build Env ${target}") {
      ditto_docker.buildDockerImage(build_config)
      ditto_docker.deleteDockerOutdated()
    }
  }

  // Master Node.
  stage("Tag and deploy?") {
    Matcher m = git_info.branch =~ "(\\d+\\.\\d+)\$"
    m.find()
    version_number = m.group(1)
    input_result = ditto_utils.promptReleaseAction(git_info,  version_number)
    def timestamp = ditto_utils.getDateTime()

    if (!git_info.branch.startsWith("release/") ||
            input_result.contains("skip")) {

      publish_repo = build_config.staging_repo
      publish_revision = "ditto~${timestamp}+git.${git_info.commit}"

    } else if (input_result.contains(" RC ")) {

      new_rc_number = calcRcNumber(version_number)
      ditto_git.pushTag("release-v${version_number}rc${new_rc_number}")
      publish_repo = build_config.staging_repo
      publish_revision = "ditto~rc{new_rc_number}"

    } else if (input_result.contains(" RELEASE ")) {

      ditto_git.pushTag("release-v${version_number}")
      publish_repo = build_config.repo
      publish_revision = "ditto"

    }
  }

  node('build && docker') {
    stage("Build and Publish to Test Repo ${target}") {
      ditto_deb.buildDebianPackage(build_config.docker_name, publish_revision)
      archiveArtifacts(artifacts: 'build/*.deb')
      ditto_deb.publishDebToS3(build_config.staging_repo, build_config.dist, S3_PACKAGE_CREDS)
    }

    stage("Publish ${target}") {
      if (publish_repo != build_config.staging_repo) {
        publishDebToS3(publish_repo, build_config.dist, S3_PACKAGE_CREDS)
      }
    }

    stage("Clean up ${target}") {
      deleteDir()
    }
  }
}
