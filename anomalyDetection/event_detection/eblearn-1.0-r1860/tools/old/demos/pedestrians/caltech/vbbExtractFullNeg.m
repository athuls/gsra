function vbbExtractFullNeg()
  out = '/home/sermanet/matlab/caltech_pedestrians/data-USA';
  name = 'usa-train';
  [pth,setIds,nVids] = dbInfo(name);
  indir = '/home/sermanet/matlab/caltech_pedestrians/data-USA/videos/';
  outdir = sprintf('%s/%s_fullneg/', out, name);
  test = 0
  if test
    outdir = sprintf('%s/%s_fullneg_test/', out, name);
  end
  display = 0

  % loop on sets
  for i=1:length(setIds)
    setId = uint32(setIds(i)) + 1;
    nVid = uint32(nVids(i));
    % loop on tracks
    for vidId=1:nVid
      s = sprintf('set%02i/V%03i', setId-1, vidId-1);
      sfile = sprintf('%s/%s', indir, s);
      sfilea = [pth '/annotations/' s];
      A = vbb('vbbLoad', sfilea);
      seqname = [indir s];
      sr = seqIo(seqname, 'reader');
      % create directories
      odir = [outdir s];
      if (~exist(odir))
	mkdir(odir);
      end
      fprintf('extracting from video file %s to %s\n', s, odir);
      % process all frames
      for iframe=1:A.nFrame
	imname = sprintf('I%05d', iframe - 1);
	fname = [ outdir '/' s '/' imname '.xml' ];
	% get frame
	sr.seek(iframe - 1);
	I = sr.getframe();
	saveBg(I, A, iframe, odir, display);
      end
    end
  end
end

function saveBg(I, A, iframe, fname, display)
% Save image after blanking all pedestrian without restriction
  for o=A.objLists{iframe}
    % global bounding box
    pos = o.pos;
    % bring indices to matlab's style
    if (pos(1) < 0) pos(1) = 0; end
    if (pos(2) < 0) pos(2) = 0; end
    pos(1) = pos(1) + 1;
    pos(2) = pos(2) + 1;
    if (uint32(pos(1)) == 0)
      fprintf('toto\n');
    end
    if (pos(3) == 0) || (pos(4) == 0)
      continue % ignore empty boxes
    end
    fprintf('    obj %i: clearing %i %i %i %i: \n', o.id, ...
	    uint32(pos(1)), uint32(pos(2)), uint32(pos(3)), uint32(pos(4)));
    % clear bbox
    range1 = [ uint32(pos(1)) : uint32(pos(1) + pos(3)) ];
    range2 = [ uint32(pos(2)) : uint32(pos(2) + pos(4)) ];
    range3 = [1, 2, 3]; 
    I(range2, range1, range3) = 0;
  end
  if (display == 1)
    subplot(1, 3, 3), imshow(I, 'Border', 'tight');
    title('pedestrian-free');
  end
  % save image
  fn = sprintf('%s/frame%05i.ppm', fname, iframe);
  imwrite(I, fn, 'ppm');
  fprintf('    bg: saved %s\n', fn);
end
