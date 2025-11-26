# Setting up TITUS (`evd.py`) for local usage

## MacOS setup

As a staring point the needed software is `brew` (installation can be [found on the site brew.sh](https://brew.sh/)), required to install a couple of tools. 

- XQuartz, downloaded with `brew install --cask xquartz`. This provides the 

# Running docker-TITUS

1. Open XQuartz with `open -a Xquartz`, then infer the `DISPLAY` port with `ps aux | grep Xquartz`, it should be something like `:0`, `:1` or any `:N`. 
2. Go to the `/path/where/is/your/data/` and start the docker with the command
```bash
docker run -it --rm -e "DISPLAY=host.docker.internal:N" -v /path/where/is/your/data/:/data sfbaylaser/titus /bin/bash
```
3. At this point setup titus and it should work...

> **Warning** :shipit: (updated Nov. 26th)
> The current `docker` image is not working correctly so for now this is NON-functioning code.