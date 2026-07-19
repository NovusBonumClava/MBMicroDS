name: Build MBMicroDS

on:
  push:
    branches: [ main ]
  workflow_dispatch:

jobs:
  build:
    runs-on: ubuntu-latest
    container:
      image: devkitpro/devkitarm:20230608

    steps:
    - uses: actions/checkout@v4

    - name: Build
      run: |
        cd MBMicroDS
        make clean
        make

    - uses: actions/upload-artifact@v4
      with:
        name: MBMicroDS
        path: MBMicroDS/MBMicroDS.nds
