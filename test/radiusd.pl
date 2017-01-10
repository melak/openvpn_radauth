#!/usr/bin/env perl

use strictures 2;

use IO::Socket;
use IO::Multiplex;

my $sock;
my $mux;

$sock = IO::Socket::INET->new(
		LocalAddr => 'localhost',
		LocalPort => 'radius',
		Proto => 'udp',
		Type => SOCK_DGRAM,
		ReuseAddr => 1,
	);
if(!$sock)
{
	die("Unable to create socket: $!");
}

$mux = IO::Multiplex->new;
$mux->add($sock);
$mux->set_callback_object('RADServer');
printf("RADIUS server started, commence testing\n");
$mux->loop;

package RADServer;

use strictures 2;

use Net::Radius::Dictionary;
use Net::Radius::Packet;

sub mux_input
{
	my $self = shift;
	my $mux = shift;
	my $sock = shift;
	my $input = shift;
	my $dict;
	my $shared_secret;

	$dict = Net::Radius::Dictionary->new;
	$dict->readfile('./dictionary.standard');
	$shared_secret = 'mekmitasdigoat';

	printf("INFO: Got new packet\n");

	while(length(${$input}) > 0)
	{
		my $rq;
		my $rp;

		$rq = Net::Radius::Packet->new($dict, ${$input});
		${$input} = '';

		printf("INFO: Received new RADIUS packet %d\n", $rq->identifier);
		printf("INFO: Packet dump:\n%s\n", $rq->str_dump);

		if($rq->code ne 'Access-Request')
		{
			printf("ERR: Expecting Access-Request packet, got %s\n", $rq->code);

			next;
		}

		$rp = Net::Radius::Packet->new($dict);
		$rp->set_attr('User-Name' => $rq->attr('User-Name'));
		$rp->set_identifier($rq->identifier);
		$rp->set_authenticator($rq->authenticator);

		if(auth($rq->attr('User-Name'), $rq->password($shared_secret, 'User-Password')) == 1)
		{
			printf("INFO: sending Access-Accept\n");
			$rp->set_code('Access-Accept');
		}
		else
		{
			printf("INFO: sending Access-Reject\n");
			$rp->set_code('Access-Reject');
		}

		printf("INFO: Reply packet:\n%s\n", $rp->str_dump);

		$sock->send(auth_resp($rp->pack, $shared_secret), 0, $mux->udp_peer($sock));

		printf("---\n");
	}
}

sub auth
{
	my $username = shift;
	my $password = shift;

	my $accounts = [
		{ username => 'okuser', password => 'okpass' },
	];

	return scalar(grep { $_->{username} eq $username && $_->{password} eq $password } @{$accounts}) == 1;
}

