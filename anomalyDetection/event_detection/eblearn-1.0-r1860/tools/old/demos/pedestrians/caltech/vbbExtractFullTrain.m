out = '/home/sermanet/matlab/caltech_pedestrians/data-USA';
name = 'usa-train';
[pth,setIds,nVids] = dbInfo(name);
indir = '/home/sermanet/matlab/caltech_pedestrians/data-USA/videos';
outdir = sprintf('%s/%s_full/', out, name);
test = 1
if test
  outdir = sprintf('%s/%s_full_test/', out, name);
end

% loop on sets
for i=1:length(setIds)
  setId = uint32(setIds(i)) + 1;
  nVid = uint32(nVids(i));
  % loop on tracks
  for vidId=1:nVid
    s = sprintf('set%02i/V%03i', setId-1, vidId-1);
    sfile = sprintf('%s/%s', indir, s);
    % create directories
    odir = [outdir s];
    if (~exist(odir))
	mkdir(odir);
      end
    fprintf('extracting from video file %s to %s\n', s, odir);
    % extract
    if test
      seqIo(sfile, 'toImgs', odir, 30, 29);
    else
      seqIo(sfile, 'toImgs', odir);
    end
  end
end