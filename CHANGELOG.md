# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [v0.1.0](https://github.com/Bit-Quill/timestream-odbc/releases/tag/v0.1.0) - 2021-02-12
### Added
- The ability to connect to Amazon Timestream using IAM and AWS Profile.
- The ability to retrieve row number from a Select query, but no support for any SQL data types yet.
### Fixed
- Memory leak in logging and query.