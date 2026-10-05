import './globals.css';
import type { Metadata } from 'next';

export const metadata: Metadata = {
  title: 'ResiliGraph Simulator | C++ Network Flow & Supply Chain Resilience Engine',
  description: 'Enterprise graph theory simulation platform computing Dinic maximum network flow, Tarjan single-point-of-failure (SPOF) cut vertices, and Monte Carlo disruption stress testing.',
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <body className="antialiased selection:bg-amber-500 selection:text-white">{children}</body>
    </html>
  );
}
