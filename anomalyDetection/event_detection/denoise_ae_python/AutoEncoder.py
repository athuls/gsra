class AutoEncoder(object):

  def __init__(self, numpy_rng, input = None, n_visible= 784, n_hidden= 500,
           W = None, bhid = None, bvis = None):
    """

    :type numpy_rng: numpy.random.RandomState
    :param numpy_rng: number random generator used to generate weights


    :type input: theano.tensor.TensorType
    :paran input: a symbolic description of the input or None for standalone
                  dA

    :type n_visible: int
    :param n_visible: number of visible units

    :type n_hidden: int
    :param n_hidden:  number of hidden units

    :type W: theano.tensor.TensorType
    :param W: Theano variable pointing to a set of weights that should be
              shared belong the dA and another architecture; if dA should
              be standalone set this to None

    :type bhid: theano.tensor.TensorType
    :param bhid: Theano variable pointing to a set of biases values (for
                 hidden units) that should be shared belong dA and another
                 architecture; if dA should be standalone set this to None

    :type bvis: theano.tensor.TensorType
    :param bvis: Theano variable pointing to a set of biases values (for
                 visible units) that should be shared belong dA and another
                 architecture; if dA should be standalone set this to None


    """
    self.n_visible = n_visible
    self.n_hidden  = n_hidden


    # note : W' was written as `W_prime` and b' as `b_prime`
    if not W:
        # W is initialized with `initial_W` which is uniformely sampled
        # from -4*sqrt(6./(n_visible+n_hidden)) and 4*sqrt(6./(n_hidden+n_visible))
        # the output of uniform if converted using asarray to dtype
        # theano.config.floatX so that the code is runable on GPU
        initial_W = numpy.asarray( numpy_rng.uniform(
                  low  = -4*numpy.sqrt(6./(n_hidden+n_visible)),
                  high =  4*numpy.sqrt(6./(n_hidden+n_visible)),
                  size = (n_visible, n_hidden)), dtype = theano.config.floatX)
        W = theano.shared(value = initial_W, name ='W')

    if not bvis:
        bvis = theano.shared(value = numpy.zeros(n_visible,
                                     dtype = theano.config.floatX), name = 'bvis')

    if not bhid:
        bhid = theano.shared(value = numpy.zeros(n_hidden,
                                          dtype = theano.config.floatX), name = 'bhid')


    self.W = W
    # b corresponds to the bias of the hidden
    self.b = bhid
    # b_prime corresponds to the bias of the visible
    self.b_prime = bvis
    # tied weights, therefore W_prime is W transpose
    self.W_prime = self.W.T
    # if no input is given, generate a variable representing the input
    if input == None :
        # we use a matrix because we expect a minibatch of several examples,
        # each example being a row
        self.x = T.dmatrix(name = 'input')
    else:
        self.x = input

    self.params = [self.W, self.b, self.b_prime]


def get_hidden_values(self, input):
    """ Computes the values of the hidden layer """
    return T.nnet.sigmoid(T.dot(input, self.W) + self.b)

def get_reconstructed_input(self, hidden ):
    """ Computes the reconstructed input given the values of the hidden layer """
    return  T.nnet.sigmoid(T.dot(hidden, self.W_prime) + self.b_prime)
