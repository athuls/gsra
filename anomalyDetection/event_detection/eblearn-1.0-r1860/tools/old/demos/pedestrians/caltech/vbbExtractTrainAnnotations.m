% add a directory link to the caltech code directory
% add your caltech code path
% addpath('/home/sermanet/matlab/caltech_pedestrians')

function vbbExtractTrainAnnotations()
  out = '/home/sermanet/matlab/caltech_pedestrians/data-USA';
  name = 'usa-train';
  [pth,setIds,nVids] = dbInfo(name);
  outdir = sprintf('%s/%s_annotations/', out, name);

  for i=1:length(setIds)
    setId = uint32(setIds(i)) + 1;
    nVid = uint32(nVids(setId));
    for vidId=1:nVid
      % load annotations
      s = sprintf('set%02i/V%03i', setId-1, vidId-1);
      fprintf('extracting from video file %s\n', s);
      sfile = [pth '/annotations/' s];
      A = vbb('vbbLoad', sfile);
      % open IO interface for this video
      seqname = [pth '/videos/' s];
      sr = seqIo(seqname, 'reader');
      % create directories
      odir = [outdir s];
      if (~exist(odir))
	mkdir(odir);
      end
      fprintf('extracting from annotation file %s to %s\n', s, odir)
      % process all frames
      for iframe=1:A.nFrame
	imname = sprintf('I%05d', iframe - 1);
	fname = [ outdir '/' s '/' imname '.xml' ];
	if (isempty(A.objLists{iframe}))
	  fprintf('  frame %d is empty\n', iframe);
	  continue ;
	end
	% get frame
	sr.seek(iframe - 1);
	I = sr.getframe();
	crop = getCrop(I);
	%imshow(I, 'Border', 'tight');
	% extract pedestrians
	saveXml(A, iframe, fname, s, imname, crop);
      end
    end
  end
end

% find how much pitch was applied to image, and set
% cropping accordingly. adjustments were only vertical, but
% black bars also appear on the left and right sides
function crop = getCrop(I)
  crop = [ 0 8 0 632 ]; % h0 w0 h1 w1
  % loop on each top row and stop when a row sum is above threshold
  i = 1;
  while i <  size(I, 1)
    if sum(I(i,:)) > 20000
      break ;
    end
    i = i + 1;
  end
  crop(1) = i - 1;
  % loop on each top row and stop when a row sum is above threshold
  i = size(I, 1);
  while i > 1
    if sum(I(i,:)) > 20000
      break ;
    end
    i = i - 1;
  end
  crop(3) = i;
end

function saveXml(A, iframe, fname, folder, imname, crop)
% Save all pedestrian images for this frame
  i = 0;
  % open file
  fp = fopen(fname, 'w'); assert(fp > 0);
  fprintf(fp, '<annotations>\n');
  fprintf(fp, '\t<folder>%s</folder>\n', folder);
  fprintf(fp, '\t<filename>%s.jpg</filename>\n', imname);
  fprintf(fp, '\t<size>\n');
  fprintf(fp, '\t\t<width>640</width>\n');
  fprintf(fp, '\t\t<height>480</height>\n');
  fprintf(fp, '\t\t<depth>3</depth>\n');
  fprintf(fp, '\t</size>\n');
  fprintf(fp, '\t<crop>\n');
  fprintf(fp, '\t\t<xmin>%f</xmin>\n', crop(1));
  fprintf(fp, '\t\t<ymin>%f</ymin>\n', crop(2));
  fprintf(fp, '\t\t<xmax>%f</xmax>\n', crop(3));
  fprintf(fp, '\t\t<ymax>%f</ymax>\n', crop(4));
  fprintf(fp, '\t</crop>\n');
  for o=A.objLists{iframe}
    lbl = A.objLbl(o.id);
    posv = o.pos; % default visibility is full
    if (o.occl) % if occluded, set actual visibility
      posv = o.posv;
    end
    fprintf(fp, '\t<object>\n');
    fprintf(fp, '\t\t<name>%s</name>\n', lbl{1});
    fprintf(fp, '\t\t<bndbox>\n');
    fprintf(fp, '\t\t\t<xmin>%f</xmin>\n', o.pos(1));
    fprintf(fp, '\t\t\t<ymin>%f</ymin>\n', o.pos(2));
    fprintf(fp, '\t\t\t<xmax>%f</xmax>\n', o.pos(1) + o.pos(3));
    fprintf(fp, '\t\t\t<ymax>%f</ymax>\n', o.pos(2) + o.pos(4));
    fprintf(fp, '\t\t</bndbox>\n');
    fprintf(fp, '\t\t<visible>\n');
    fprintf(fp, '\t\t\t<xmin>%f</xmin>\n', posv(1));
    fprintf(fp, '\t\t\t<ymin>%f</ymin>\n', posv(2));
    fprintf(fp, '\t\t\t<xmax>%f</xmax>\n', posv(1) + posv(3));
    fprintf(fp, '\t\t\t<ymax>%f</ymax>\n', posv(2) + posv(4));
    fprintf(fp, '\t\t</visible>\n');
    fprintf(fp, '\t</object>\n');
    i = i + 1;
  end
  fprintf(fp, '</annotations>\n');
  fclose(fp);
  fprintf('  extracted %d objects to %s\n', i, fname);
end
