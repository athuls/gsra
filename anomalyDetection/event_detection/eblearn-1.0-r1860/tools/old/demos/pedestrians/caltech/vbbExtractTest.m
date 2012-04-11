% add a directory link to the caltech code directory
% add your caltech code path
% addpath('/home/sermanet/matlab/caltech_pedestrians')
% addpath('/home/sermanet/eblpierre/tools/demos/pedestrians/caltech')

% Extract all test images.
% vbbExtract('usa-test', 1, 30, 30)

function vbbExtractTest(name, display, period, initial_frame)
% Extract all test images
% by Pierre Sermanet
% INPUTS
%   display - if 1, display which windows are extracted.
%   period - take a frame each period frames
%   intial_frame - the first frame take has id initial_frame
  
  % default values
  if (nargin < 4) initial_frame = 30; end
  if (nargin < 3) period = 30; end
  if (nargin < 2) display = 0; end
  if (nargin < 1) name = 'usa-test'; end

  [pth,setIds,nVids] = dbInfo(name);
  extracted = sprintf('test_period%i_init%i', period, initial_frame);
  outdir = [pth '/' extracted '/'];
  for i=1:length(setIds)
    setId = uint32(setIds(i)) + 1;
    nVid = uint32(nVids(i));
    for vidId=1:nVid
      vStr = sprintf('set%02i/V%03i', setId-1, vidId-1);
      fprintf('extracting from video file %s\n', vStr);
      % open IO interface for this video
      seqname = [pth '/videos/' vStr];
      sr = seqIo(seqname, 'reader');
      % create directories
      fname = [outdir vStr];
      if (~exist(fname))
	mkdir(fname);
      end
      % process all frames
      iframe = initial_frame - 1;
      while (sr.seek(iframe) ~= 0)
	fprintf('  frame %i\n', iframe);
	% get frame
	I = sr.getframe();
	% display original annotations
	if (display == 1)
	  subplot(1, 3, 1), imshow(I, 'Border', 'tight');
	end
	% save image
	fn = sprintf('%s/frame%05i.ppm', fname, uint32(iframe));
	imwrite(I, fn, 'ppm');
	fprintf('    saved %s\n', fn);
	iframe = iframe + uint32(period);
      end
    end
  end
end
